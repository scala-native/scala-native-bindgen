#include "ScalaBindgen.h"


#include "clang/Tooling/Tooling.h"
#include "catch/catch.hpp"

#include <iostream>

std::string Translate(std::string code){
    declarations = "";
    enums = "";
    auto* action = new ExampleFrontendAction;
    clang::tooling::runToolOnCode(action, code);
    return declarations;
}

TEST_CASE("native types", "[Type]" ) {

    std::map<std::string, std::string> types = {
        {"void", "Unit"},
        //{"bool", "native.CBool"},
        {"char", "native.CChar"},
        {"signed char", "native.CSignedChar"},
        {"unsigned char", "native.CUnsignedChar"},
        {"short", "native.CShort"},
        {"unsigned short", "native.CUnsignedShort"},
        {"int", "native.CInt"},
        //{"long int", "native.CLongInt"},
        {"unsigned int", "native.CUnsignedInt"},
        //{"unsigned long int", "native.CUnsignedLongInt"},
        {"long", "native.CLong"},
        {"unsigned long", "native.CUnsignedLong"},
        {"long long", "native.CLongLong"},
        {"unsigned long long", "native.CUnsignedLongLong"},
        //{"size_t", "native.CSize"},
        //{"ptrdiff_t", "native.CPtrDiff"},
        {"wchar_t", "native.CWideChar"},
        {"char16_t", "native.CChar16"},
        {"char32_t", "native.CChar32"},
        {"float", "native.CFloat"},
        {"double", "native.CDouble"},
        {"void*", "native.Ptr[Byte]"},
        {"int*", "native.Ptr[native.CInt]"},
        //{"char*", "native.CString"},
        //{"int (*a)(int)", "native.CFunctionPtr1[native.CInt, native.CInt]"},
        //{"struct { int x, y; }*", "native.Ptr[native.CStruct2[native.CInt, native.CInt]]"}
    };

    for(const auto& kv: types){
        std::string code = "typedef " + kv.first + " a;\n";
        std::string answer = "\ttype a = " + kv.second + "\n";

        REQUIRE(answer == Translate(code));
    }

}
