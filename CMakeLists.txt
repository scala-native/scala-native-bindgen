set(LLVM_LINK_COMPONENTS support)

add_clang_executable(scalaBindgen
  scalaBindgen.cpp
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