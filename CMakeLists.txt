cmake_minimum_required(VERSION 3.9)
project(scala-native-bindgen)

# Locate $LLVM_PATH/lib/cmake/clang/ClangConfig.cmake
find_package(Clang REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_compile_options(-fexceptions -std=c++11)

add_library(scalaNativeBindgen
  STATIC
  ScalaFrontend.h
  TreeVisitor.h
  TreeVisitor.cpp
  TreeConsumer.h
  TypeTranslator.h
  TypeTranslator.cpp
  HeaderManager.h
  HeaderManager.cpp
  CycleDetection.h
  Utils.h
  )

add_executable(scalaBindgen
  Main.cpp
  )

target_link_libraries(scalaBindgen
  PRIVATE
  scalaNativeBindgen
  clangFrontend
  clangTooling
  )
