cmake_minimum_required(VERSION 3.9)
project(scala-native-bindgen)

# Locate $LLVM_PATH/lib/cmake/clang/ClangConfig.cmake
find_package(Clang REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_compile_options(-fexceptions -std=c++11 -Wall -Wconversion -Werror)

add_executable(bindgen
  Main.cpp
  visitor/ScalaFrontendAction.h
  visitor/ScalaFrontendAction.cpp
  visitor/ScalaFrontendActionFactory.h
  visitor/ScalaFrontendActionFactory.cpp
  visitor/TreeVisitor.h
  visitor/TreeVisitor.cpp
  visitor/TreeConsumer.h
  TypeTranslator.h
  TypeTranslator.cpp
  HeaderManager.h
  HeaderManager.cpp
  CycleDetection.h
  Utils.h
  ir/IR.h
  ir/IR.cpp
  ir/Struct.cpp
  ir/Struct.h
  ir/Function.cpp
  ir/Function.h
  ir/TypeDef.cpp
  ir/TypeDef.h
  ir/Enum.cpp
  ir/Enum.h
  ir/TypeAndName.cpp
  ir/TypeAndName.h
)

set_target_properties(bindgen
  PROPERTIES
  OUTPUT_NAME scala-native-bindgen
)

target_link_libraries(bindgen
  PRIVATE
  clangFrontend
  clangTooling
)
