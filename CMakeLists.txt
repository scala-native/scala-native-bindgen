set(LLVM_LINK_COMPONENTS support)

add_compile_options(-fexceptions)

add_clang_executable(scalaBindgen
  Main.cpp
  ScalaFrontend.h
  TreeVisitor.h
  TreeVisitor.cpp
  TreeConsumer.h
  TypeTranslator.h
  TypeTranslator.cpp
  HeaderManager.h
  HeaderManager.cpp
  Utils.h
  catch/catch.hpp
  SimpleTypeTests.cpp
  )

target_link_libraries(scalaBindgen
  PRIVATE
  clangFrontend
  clangSerialization
  clangDriver
  clangTooling
  clangParse
  clangSema
  
  clangAnalysis
  
  clangEdit
  clangAST
  clangLex
  clangBasic
  )
