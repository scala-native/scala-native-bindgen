#include "clang/Tooling/Tooling.h"
#include "gtest/gtest.h"

#include "scalaBindgen.cpp"

TEST(runToolOnCode, CanSyntaxCheckCode) {
  EXPECT_TRUE(runToolOnCode(new clang::SyntaxOnlyAction, "class X {};"));
}