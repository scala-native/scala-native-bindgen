cmake_minimum_required(VERSION 3.9)
project(scala-native-bindgen)

# Locate $LLVM_PATH/lib/cmake/clang/ClangConfig.cmake
find_package(Clang REQUIRED CONFIG)

# Read and set CXXFLAGS and LDFLAGS
execute_process(COMMAND llvm-config --cxxflags
  OUTPUT_VARIABLE LLVM_CXX_FLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

execute_process(COMMAND llvm-config --ldflags
  OUTPUT_VARIABLE LLVM_LINKER_FLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LINKER_FLAGS}")

add_compile_options(-fexceptions)

add_executable(scalaBindgen
  Main.cpp
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
  catch/catch.hpp
  SimpleTypeTests.cpp
  )

target_link_libraries(scalaBindgen
  PRIVATE
  clangFrontend
  clangTooling
  )
