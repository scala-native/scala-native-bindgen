set(LLVM_LINK_COMPONENTS support)

add_clang_executable(scalaBindgen
  ScalaBindgen.cpp
  TypeTranslator.h
  TypeTranslator.cpp
  HeaderManager.h
  HeaderManager.cpp
  Utils.h
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
