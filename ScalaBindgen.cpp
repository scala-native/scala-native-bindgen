#include "ScalaBindgen.h"

#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"


#define SCALA_NATIVE_MAX_STRUCT_FIELDS 22


static llvm::cl::OptionCategory Category("Binding Generator");
static llvm::cl::extrahelp CommonHelp(clang::tooling::CommonOptionsParser::HelpMessage);
static llvm::cl::extrahelp MoreHelp("\nProduce Bindings for scala native. Please specify lib name wit parameter name\n");
static llvm::cl::opt<std::string> LibName("name", llvm::cl::cat(Category));
static llvm::cl::opt<std::string> StdHeaders("stdHeaders", llvm::cl::cat(Category));


HeaderManager headerMan;

std::string declarations;
std::string enums;
std::string helpers;

bool TreeVisitor::VisitFunctionDecl(clang::FunctionDecl *func) {
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

    declarations += "\tdef " + funcName + "(" + params + "): " + retType + " = native.extern\n";
    return true;
}

bool TreeVisitor::VisitTypedefDecl(clang::TypedefDecl *tpdef){
    std::string name = tpdef->getName();
    std::string tpe = typeTranslator.Translate(tpdef->getUnderlyingType());
    declarations += "\ttype " + name + " = " + tpe + "\n";
    return true;
}

bool TreeVisitor::VisitEnumDecl(clang::EnumDecl *enumdecl){
    std::string name = enumdecl->getNameAsString();

    //Replace "enum x" with enum_x in scala
    typeTranslator.AddTranslation("enum " + name, "enum_" + name);

    if(name == "" && enumdecl->getTypedefNameForAnonDecl()){
        name = enumdecl->getTypedefNameForAnonDecl()->getNameAsString();
    }

    if(name != ""){
     declarations += "\ttype enum_" + name + " = native.CInt\n";
    }

    int i = 0;
    for (const clang::EnumConstantDecl* en : enumdecl->enumerators()){
        if(name != ""){
            enums += "\tfinal val enum_" + name + "_" + en->getNameAsString() + " = " + std::to_string(i++) + "\n";
        } else {
            enums += "\tfinal val enum_" + en->getNameAsString() + " = " + std::to_string(i++) + "\n";
        }
    }

    return true;
}

bool TreeVisitor::VisitRecordDecl(clang::RecordDecl *record){
    std::string name = record->getNameAsString();

    //Handle typedef struct {} x; and typedef union {} y; by getting the name from the typedef
    if((record->isStruct() || record->isUnion()) && name == "" && record->getTypedefNameForAnonDecl()){
        name = record->getTypedefNameForAnonDecl()->getNameAsString();
    }

    if(record->isUnion() && !record->isAnonymousStructOrUnion() && name != ""){

        //Replace "union x" with union_x in scala
        typeTranslator.AddTranslation("union " + name, "union" + name);

        uint64_t maxSize = 0;

        for(const clang::FieldDecl* field : record->fields()){
            maxSize = std::max(maxSize, astContext->getTypeSize(field->getType()));
        }

        declarations += "\ttype union_" + name + " = native.CArray[Byte, " + intToScalaNat(maxSize) + "]\n";

        return true;

    } else if (record->isStruct() && record->isThisDeclarationADefinition() && !record->isAnonymousStructOrUnion() && name != ""){

        //Replace "struct x" with struct_x in scala
        typeTranslator.AddTranslation("struct " + name, "struct_"+name);

        int fieldCnt = 0;
        std::string fields = "";
        std::string helpersFunc = "";

        for(const clang::FieldDecl* field : record->fields()){
            std::string fname = field->getNameAsString();
            std::string ftype = typeTranslator.Translate(field->getType(), &name);

            fields += ftype + ", ";
            if(name != ""){
                helpersFunc += "\t\tdef " + fname + ": " + ftype + " = !p._" + std::to_string(fieldCnt + 1) + "\n";
                helpersFunc += "\t\tdef " + fname +"_=(value: " + ftype + "):Unit = !p._" + std::to_string(fieldCnt + 1) + " = value\n";
            }
            fieldCnt++;
        }

        //remove last ,
        if(fields != ""){
            fields = fields.substr(0, fields.size()-2);
        }

        if(fieldCnt < SCALA_NATIVE_MAX_STRUCT_FIELDS){
            declarations += "\ttype struct_" + name + " = " + "native.CStruct" + std::to_string(fieldCnt) + "[" + fields + "]\n";
        } else {
            //There is no easy way to represent it as a struct in scala native, have to represent it as an array and then
            //Add helpers to help with it's manipulation
            uint64_t size = astContext->getTypeSize(record->getTypeForDecl());
            declarations += "\ttype struct_" + name + " = " + "native.CArray[Byte, " + uint64ToScalaNat(size) + "]\n";
        }

        //Create helpers in an implicit class
        if(fieldCnt > 0 && fieldCnt < SCALA_NATIVE_MAX_STRUCT_FIELDS){
            helpers += "\timplicit class struct_" + name + "_ops(val p: native.Ptr[struct_" + name + "]) extends AnyVal {\n";
            helpers += helpersFunc;
            helpers += "\t}\n\n";
        }

        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {

    if(argc <= 1 ){

        int result = Catch::Session().run( argc, argv );
        return result;

    } else{

        clang::tooling::CommonOptionsParser op(argc, (const char**)argv, Category);
        clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

        auto lib = LibName.getValue();
        if(lib == ""){
            llvm::errs() << "Error: Please specify the lib name using -name paramter\n";
            return -1;
        }

        auto stdhead = StdHeaders.getValue();
        if(stdhead != ""){
            headerMan.LoadConfig(stdhead);
        }

        declarations = "";
        enums = "";
        helpers = "";

        int result = Tool.run(clang::tooling::newFrontendActionFactory<ExampleFrontendAction>().get());

        llvm::outs() << "import scala.scalanative._\n"
                     << "import scala.scalanative.native.Nat._\n\n";

        if(declarations != ""){
            llvm::outs() << "@native.link(\"" << lib << "\")\n"
                         << "@native.extern\n"
                         << "object " << lib << " {\n"
                         << declarations
                         << "}\n\n"
                         << "import " + lib + "._\n\n";
        }

        if(enums != ""){
            llvm::outs() << "object " << lib << "Enums {\n"
                         << enums
                         << "}\n\n";
        }

        if(helpers != ""){
            llvm::outs() << "object " << lib << "Helpers {\n"
                         << helpers
                         << "}\n\n";
        }

        return result;
    }
}
