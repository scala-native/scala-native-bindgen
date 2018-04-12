#include "TypeTranslator.h"

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

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;


static llvm::cl::OptionCategory Category("Binding Generator");
static llvm::cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name wit parameter name");
static llvm::cl::opt<std::string> LibName("name", cl::cat(Category));


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
            params += parm->getNameAsString();
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
    	std::string enumName = enumdecl->getNameAsString();

		//Replace "enum x" with enum_x in scala
		typeTranslator.AddTranslation("enum " + enumName, "enum_" + enumName);

        llvm::outs() << "\ttype enum_" << enumName << " = native.CInt\n";

    	int i = 0;
    	for (const EnumConstantDecl* en : enumdecl->enumerators()){
            llvm::outs() << "\tval enum_" << enumName << "_" << en->getNameAsString() << " = " << i++ << "\n";
    	}

    	return true;
    }

    virtual bool VisitRecordDecl(clang::RecordDecl *record){
        std::string name = record->getNameAsString();

        //Handle typedef struct {} x; by getting the name from the type
        if((record->isStruct() || record->isUnion()) && name == ""){
            const clang::RecordType* rec = record->getTypeForDecl()->getAsStructureType();
            clang::Qualifiers q{};
            name =  QualType::getAsString(rec, q,clang::LangOptions());
        }

        if(record->isUnion() && !record->isAnonymousStructOrUnion() && name != ""){

    		//Replace "union x" with union_x in scala
    		typeTranslator.AddTranslation("union " + name, "union" + name);
    	
    		uint64_t maxSize = 0;

    		for(const FieldDecl* field : record->fields()){
    			maxSize = std::max(maxSize, astContext->getTypeSize(field->getType()));
    		}

    		llvm::outs() << "\ttype union_" << name << " = native.CArray[native.Byte, " << maxSize << "]\n"; 

      		return true;
        } else if (record->isStruct() && !record->isAnonymousStructOrUnion() && name != ""){

    		//Replace "struct x" with struct_x in scala
    		typeTranslator.AddTranslation("struct " + name, "struct_"+name);

    		int counter = 0;
    		std::string fields = "";

    		for(const FieldDecl* field : record->fields()){
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

public:
    explicit TreeConsumer(CompilerInstance *CI) : visitor(new TreeVisitor(CI)) {}

    // override this to call our TreeVisitor on each top-level Decl
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            Decl *D = *i;    
            visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
        }
        return true;
    }

};



class ExampleFrontendAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return std::unique_ptr<clang::ASTConsumer>(new TreeConsumer(&CI)); // pass CI pointer to ASTConsumer
    }
};



int main(int argc, const char **argv) {
    CommonOptionsParser op(argc, argv, Category);        
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto lib = LibName.getValue();
    if(lib == ""){
    	llvm::errs() << "Error: Please specify the lib name using -name paramter\n";
    	return -1;
    }

    llvm::outs() << "@native.extern\nobject " << lib<< " {\n";
    int result = Tool.run(newFrontendActionFactory<ExampleFrontendAction>().get());
    llvm::outs() << "}\n";

    return result;
}
