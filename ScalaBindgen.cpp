#include "TypeTranslator.h"
#include "Utils.h"

#include "clang/Driver/Options.h"
#include "clang/Basic/LangOptions.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <iostream>

static llvm::cl::OptionCategory Category("Binding Generator");
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name wit parameter name\n");
static llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category));


class TreeVisitor : public clang::RecursiveASTVisitor<TreeVisitor> {
private:
    clang::ASTContext* astContext;
    TypeTranslator typeTranslator;

public:
    explicit TreeVisitor(clang::CompilerInstance *CI) : astContext(&(CI->getASTContext())), typeTranslator(astContext) {}

    virtual bool VisitFunctionDecl(clang::FunctionDecl *func) {
        std::string funcName = func->getNameInfo().getName().getAsString();
        std::string retType = typeTranslator.Translate(func->getReturnType());
        std::string params = "";

        for (const clang::ParmVarDecl* parm : func->parameters()){
    		//Handle default values
            std::string pname = parm->getNameAsString();
            if(pname == ""){
                pname = "anonymous";
            }
            params += pname;
    		params += ": ";
    		params += typeTranslator.Translate(parm->getType());
    		params += ", ";
    	}

    	//remove last ,
    	if(params != ""){
    		params = params.substr(0, params.size()-2);
    	}
      	
        llvm::outs() << "\tdef " << funcName << "(" << params << "): " + retType + " = native.extern\n";
        return true;
    }

    virtual bool VisitTypedefDecl(clang::TypedefDecl *tpdef){
		//TODO: Understand difference between typedef and typedef-name
		std::string name = tpdef->getName();
        std::string tpe = typeTranslator.Translate(tpdef->getUnderlyingType());
        //clang::QualType type = tpdef->getUnderlyingType();
        //llvm::errs() << type.isAtLeastAsQualifiedAs( << "\n";
    	llvm::outs() << "\ttype " << name << " = " << tpe << "\n";
    	return true;
    }

    virtual bool VisitEnumDecl(clang::EnumDecl *enumdecl){
        std::string name = enumdecl->getNameAsString();

        //Handle typedef enum {} x; by getting the name from the type
        if(name == ""){
            const clang::EnumType* rec = enumdecl->getTypeForDecl()->getAs<clang::EnumType>();
            clang::Qualifiers q{};
            name = clang::QualType::getAsString(rec, q,clang::LangOptions());
        }

		//Replace "enum x" with enum_x in scala
        typeTranslator.AddTranslation("enum " + name, "enum_" + name);

        llvm::outs() << "\ttype enum_" << name << " = native.CInt\n";

    	int i = 0;
        for (const clang::EnumConstantDecl* en : enumdecl->enumerators()){
            llvm::outs() << "\tval enum_" << name << "_" << en->getNameAsString() << " = " << i++ << "\n";
    	}

    	return true;
    }

    virtual bool VisitRecordDecl(clang::RecordDecl *record){
        std::string name = record->getNameAsString();

        //Handle typedef struct {} x; by getting the name from the type
        if((record->isStruct()) && name == ""){
            const clang::RecordType* rec = record->getTypeForDecl()->getAsStructureType();
            clang::Qualifiers q{};
            name = clang::QualType::getAsString(rec, q,clang::LangOptions());

            //TODO find a better way
            size_t f = name.find("anonymous at");
            if(f != std::string::npos){
                return true;
            }
        }

        //Handle typedef union {} x; by getting the name from the type
        if((record->isUnion()) && name == ""){
            const clang::RecordType* rec = record->getTypeForDecl()->getAsUnionType();
            clang::Qualifiers q{};
            name = clang::QualType::getAsString(rec, q,clang::LangOptions());

            //TODO find a better way
            size_t f = name.find("anonymous at");
            if(f != std::string::npos){
                return true;
            }
        }


        if(record->isUnion() && !record->isAnonymousStructOrUnion() && name != ""){

    		//Replace "union x" with union_x in scala
    		typeTranslator.AddTranslation("union " + name, "union" + name);
    	
    		uint64_t maxSize = 0;

            for(const clang::FieldDecl* field : record->fields()){
    			maxSize = std::max(maxSize, astContext->getTypeSize(field->getType()));
    		}

            llvm::outs() << "\ttype union_" << name << " = native.CArray[Byte, " << intToScalaNat(maxSize) << "]\n";

      		return true;
        } else if (record->isStruct() && record->isThisDeclarationADefinition() && !record->isAnonymousStructOrUnion() && name != ""){

    		//Replace "struct x" with struct_x in scala
    		typeTranslator.AddTranslation("struct " + name, "struct_"+name);

    		int counter = 0;
    		std::string fields = "";

            for(const clang::FieldDecl* field : record->fields()){
    			fields += typeTranslator.Translate(field->getType()) + ",";
    			counter++;
    		}

	    	//remove last ,
	    	if(fields != ""){
	    		fields = fields.substr(0, fields.size()-1);
	    	}


    		llvm::outs() << "\ttype struct_" << name << " = " << "native.CStruct" << counter << "[" << fields << "]\n";

	    	return true;
    	}
    	return false;
    }

};



class TreeConsumer : public clang::ASTConsumer {
private:
    TreeVisitor *visitor;
    clang::SourceManager& smanager;

public:

    std::vector<std::string> stdheaders{"assert.h","complex.h","ctype.h","errno.h","fenv.h","float.h","inttypes.h","iso646.h","limits.h",
                                        "locale.h","math.h","setjmp.h","signal.h","stdalign.h","stdarg.h","stdatomic.h","stdbool.h","stddef.h",
                                        "stdint.h","stdio.h","stdlib.h","stdnoreturn.h","string.h","tgmath.h","threads.h","time.h","uchar.h",
                                        "wchar.h","wctype.h","aio.h","inet.h","assert.h","complex.h","cpio.h","ctype.h","dirent.h","dlfcn.h",
                                        "errno.h","fcntl.h","fenv.h","float.h","fmtmsg.h","fnmatch.h","ftw.h","glob.h","grp.h","iconv.h",
                                        "inttypes.h","iso646.h","langinfo.h","libgen.h","limits.h","locale.h","math.h","monetary.h","mqueue.h",
                                        "ndbm.h","if.h","netdb.h","in.h","tcp.h","nl_types.h","poll.h","pthread.h","pwd.h",
                                        "regex.h","sched.h","search.h","semaphore.h","setjmp.h","signal.h","spawn.h","stdarg.h","stdbool.h",
                                        "stddef.h","stdint.h","stdio.h","stdlib.h","string.h","strings.h","stropts.h","ipc.h","mman.h",
                                        "msg.h","resource.h","select.h","sem.h","shm.h","socket.h","stat.h",
                                        "statvfs.h","time.h","times.h","types.h","uio.h","un.h","utsname.h",
                                        "wait.h","syslog.h","tar.h","termios.h","tgmath.h","time.h","trace.h","ulimit.h","unistd.h",
                                        "utime.h","utmpx.h","wchar.h","wctype.h","wordexp.h",

                                        "siginfo.h", "sigset.h", "sigaction.h", "sigcontext.h", "sigthread.h", "ucontext.h",
                                        "pthreadtypes.h", "libio.h", "__stddef_max_align_t.h", "_G_config.h", "sys_errlist.h",
                                        "sysmacros.h", "xlocale.h", "socket_type.h", "sockaddr.h", "sigstack.h"
                                       };

    /* with folders
    std::vector<std::string> stdheaders{"assert.h","complex.h","ctype.h","errno.h","fenv.h","float.h","inttypes.h","iso646.h","limits.h",
                                        "locale.h","math.h","setjmp.h","signal.h","stdalign.h","stdarg.h","stdatomic.h","stdbool.h","stddef.h",
                                        "stdint.h","stdio.h","stdlib.h","stdnoreturn.h","string.h","tgmath.h","threads.h","time.h","uchar.h",
                                        "wchar.h","wctype.h","aio.h","arpa/inet.h","assert.h","complex.h","cpio.h","ctype.h","dirent.h","dlfcn.h",
                                        "errno.h","fcntl.h","fenv.h","float.h","fmtmsg.h","fnmatch.h","ftw.h","glob.h","grp.h","iconv.h",
                                        "inttypes.h","iso646.h","langinfo.h","libgen.h","limits.h","locale.h","math.h","monetary.h","mqueue.h",
                                        "ndbm.h","net/if.h","netdb.h","netinet/in.h","netinet/tcp.h","nl_types.h","poll.h","pthread.h","pwd.h",
                                        "regex.h","sched.h","search.h","semaphore.h","setjmp.h","signal.h","spawn.h","stdarg.h","stdbool.h",
                                        "stddef.h","stdint.h","stdio.h","stdlib.h","string.h","strings.h","stropts.h","sys/ipc.h","sys/mman.h",
                                        "sys/msg.h","sys/resource.h","sys/select.h","sys/sem.h","sys/shm.h","sys/socket.h","sys/stat.h",
                                        "sys/statvfs.h","sys/time.h","sys/times.h","sys/types.h","sys/uio.h","sys/un.h","sys/utsname.h",
                                        "sys/wait.h","syslog.h","tar.h","termios.h","tgmath.h","time.h","trace.h","ulimit.h","unistd.h",
                                        "utime.h","utmpx.h","wchar.h","wctype.h","wordexp.h"};
     */

    explicit TreeConsumer(clang::CompilerInstance *CI) : visitor(new TreeVisitor(CI)), smanager(CI->getASTContext().getSourceManager()) {}

    std::string basename(const std::string& pathname) {
        return {std::find_if(pathname.rbegin(), pathname.rend(),
                             [](char c) { return c == '/'; }).base(),
                pathname.end()};
    }

    virtual bool HandleTopLevelDecl(clang::DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (clang::DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            clang::Decl *D = *i;
            std::string fpath = smanager.getFilename(D->getLocation()).str();
            if(std::find(stdheaders.begin(), stdheaders.end(), basename(fpath)) == stdheaders.end()){
                visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
            }
        }
        return true;
    }

    // this replaces "HandleTopLevelDecl"
    // override this to call our ExampleVisitor on the entire source file
    /*virtual void HandleTranslationUnit(clang::ASTContext &Context) {
        //we can use ASTContext to get the TranslationUnitDecl, which is
        //a single Decl that collectively represents the entire source file
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }*/

};



class ExampleFrontendAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, clang::StringRef file) {
        return std::unique_ptr<clang::ASTConsumer>(new TreeConsumer(&CI)); // pass CI pointer to ASTConsumer
    }
};


int main(int argc, const char **argv) {
    clang::tooling::CommonOptionsParser op(argc, argv, Category);
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto lib = LibName.getValue();
    if(lib == ""){
    	llvm::errs() << "Error: Please specify the lib name using -name paramter\n";
    	return -1;
    }

    llvm::outs() << "import scala.scalanative._\n";
    llvm::outs() << "import scala.scalanative.native.Nat._\n\n";
    llvm::outs() << "@native.link(\"" << lib << "\")\n";
    llvm::outs() << "@native.extern\nobject " << lib<< " {\n";
    int result = Tool.run(clang::tooling::newFrontendActionFactory<ExampleFrontendAction>().get());
    llvm::outs() << "}\n";

    return result;
}
