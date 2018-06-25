cmake_minimum_required(VERSION 3.9)
project(scala-native-bindgen)

# Locate LLVMConfig.cmake
#find_package(LLVM REQUIRED CONFIG)
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_program(LLVM_CONFIG_PROGRAM
  NAMES
#  llvm-config-${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}
  llvm-config-6.0
  llvm-config-5.0
  llvm-config-4.0
  llvm-config
)

execute_process(COMMAND ${LLVM_CONFIG_PROGRAM} --includedir
  OUTPUT_VARIABLE LLVM_INCLUDE_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Using LLVM include dir: ${LLVM_INCLUDE_DIR}")

execute_process(COMMAND ${LLVM_CONFIG_PROGRAM} --cxxflags
  OUTPUT_VARIABLE LLVM_CXX_FLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Using LLVM CXXFLAGS: ${LLVM_CXX_FLAGS}")

execute_process(COMMAND ${LLVM_CONFIG_PROGRAM} --ldflags
  OUTPUT_VARIABLE LLVM_LINKER_FLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Using LLVM LDFLAGS: ${LLVM_LINKER_FLAGS}")

execute_process(COMMAND ${LLVM_CONFIG_PROGRAM} --libs --link-static
  OUTPUT_VARIABLE LLVM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND ${LLVM_CONFIG_PROGRAM} --system-libs --link-static
  OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

include_directories(SYSTEM ${LLVM_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LLVM_CXX_FLAGS}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # macOS does not guarantee backwards compatible system calls and therefore
  # discourages statically linked binaries. Instead add -L/usr/lib before the
  # LLVM LDFLAGS to link against the dynamic system libc++ instead of the one
  # from LLVM.
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib ${LLVM_LINKER_FLAGS}")
else()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LLVM_LINKER_FLAGS} -static")
endif()

add_compile_options(-Wall -Wconversion)

add_executable(bindgen
  Main.cpp
  visitor/ScalaFrontendAction.h
  visitor/ScalaFrontendAction.cpp
  visitor/ScalaFrontendActionFactory.h
  visitor/ScalaFrontendActionFactory.cpp
  visitor/TreeVisitor.h
  visitor/TreeVisitor.cpp
  visitor/TreeConsumer.h
  defines/DefineFinderAction.h
  defines/DefineFinderAction.cpp
  defines/DefineFinder.cpp
  defines/DefineFinder.h
  defines/DefineFinderActionFactory.cpp 
  defines/DefineFinderActionFactory.h
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
  ir/Define.h
  ir/Define.cpp
  ir/LiteralDefine.cpp
  ir/LiteralDefine.h
  ir/VarDefine.cpp
  ir/VarDefine.h
  ir/Variable.cpp
  ir/Variable.h
  ir/PossibleVarDefine.cpp
  ir/PossibleVarDefine.h
  ir/types/Type.cpp
  ir/types/Type.h
  ir/types/PrimitiveType.cpp
  ir/types/PrimitiveType.h
  ir/types/PointerType.cpp
  ir/types/PointerType.h
  ir/types/FunctionPointerType.cpp
  ir/types/FunctionPointerType.h
  ir/types/ArrayType.cpp
  ir/types/ArrayType.h
)

set_target_properties(bindgen
  PROPERTIES
  OUTPUT_NAME scala-native-bindgen
)

target_link_libraries(bindgen
  PRIVATE
  clangFrontend
  clangTooling

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

  "${LLVM_LIBS}"
  "${LLVM_SYSTEM_LIBS}"
)
