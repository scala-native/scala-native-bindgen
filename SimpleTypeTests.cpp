#include "ScalaFrontend.h"

#include "clang/Tooling/Tooling.h"
#include "catch/catch.hpp"

#include <iostream>

std::string Translate(std::string code){
    declarations = "";
    enums = "";
    helpers = "";
    auto* action = new ScalaFrontendAction;
    clang::tooling::runToolOnCode(action, code, "input.h");
    return declarations;
}

TEST_CASE("native types", "[Type]" ) {

    std::map<std::string, std::string> types = {
        {"void", "Unit"},
        {"char", "native.CChar"},
        {"signed char", "native.CSignedChar"},
        {"unsigned char", "native.CUnsignedChar"},
        {"short", "native.CShort"},
        {"unsigned short", "native.CUnsignedShort"},
        {"int", "native.CInt"},
        {"long", "native.CLong"},
        //{"long int", "native.CLongInt"}, <- similar to a long, why is it different ?
        {"unsigned int", "native.CUnsignedInt"},
        {"unsigned long", "native.CUnsignedLong"},
        //{"unsigned long int", "native.CUnsignedLongInt"}, <- similar to an unsigned long why is it different ?
        {"long long", "native.CLongLong"},
        {"unsigned long long", "native.CUnsignedLongLong"},
        {"size_t", "native.CSize"},
        {"ptrdiff_t", "native.CPtrDiff"},
        {"wchar_t", "native.CWideChar"},
        {"char16_t", "native.CChar16"},
        {"char32_t", "native.CChar32"},
        {"float", "native.CFloat"},
        {"double", "native.CDouble"},
        {"void*", "native.Ptr[Byte]"},
        {"int*", "native.Ptr[native.CInt]"},
        {"char*", "native.CString"},
    };

    for(const auto& kv: types){
        std::string code = "#include<uchar.h>\n#include<stddef.h>\ntypedef " + kv.first + " a;\n";
        std::string answer = "\ttype a = " + kv.second + "\n";

        REQUIRE(answer == Translate(code));
    }
 }

TEST_CASE("native types function pointer", "[Type]"){
    std::string code = "typedef int (*a)(int);";
    std::string answ = "\ttype a = native.CFunctionPtr1[native.CInt, native.CInt]\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("struct pointer", "[Type]"){
    std::string code = "struct asd { int x, y; };"
                       "typedef struct asd* ptre;";
    std::string answ = "\ttype struct_asd = native.CStruct2[native.CInt, native.CInt]\n"
                       "\ttype ptre = native.Ptr[struct_asd]\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("func no args", "[Func]"){
    std::string code = "int foo();";
    std::string answ = "\tdef foo(): native.CInt = native.extern\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("func void args", "[Func]"){
    std::string code = "int foo(void);";
    std::string answ = "\tdef foo(): native.CInt = native.extern\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("func 1 arg", "[Func]"){
    std::string code = "void foo(int a);";
    std::string answ = "\tdef foo(a: native.CInt): Unit = native.extern\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("func 2 args", "[Func]"){
    std::string code = "void foo(float a, int b);";
    std::string answ = "\tdef foo(a: native.CFloat, b: native.CInt): Unit = native.extern\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("func anonymous args", "[Func]"){
    std::string code = "void foo(float, int);";
    std::string answ = "\tdef foo(anonymous0: native.CFloat, anonymous1: native.CInt): Unit = native.extern\n";
    REQUIRE(answ == Translate(code));
}

TEST_CASE("func variadic args", "[Func]"){
    std::string code = "double foo(double a, void* b, ...);";
    std::string answ = "\tdef foo(a: native.CDouble, b: native.Ptr[Byte], varArgs: native.CVararg*): native.CDouble = native.extern\n";
    REQUIRE(answ == Translate(code));
}


