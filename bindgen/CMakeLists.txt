cmake_minimum_required(VERSION 3.9)
project(scala-native-bindgen)

option(STATIC_LINKING "Statically link the executable" OFF)

# Locate LLVMConfig.cmake
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message(STATUS "Using LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

message(STATUS "Using LLVM defs: ${LLVM_DEFINITIONS}")
add_definitions(${LLVM_DEFINITIONS})

message(STATUS "Using LLVM library directories: ${LLVM_LIBRARY_DIRS}")
link_directories(${LLVM_LIBRARY_DIRS})

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
  ir/location/Location.h
  ir/location/ScalaLocation.cpp
  ir/location/ScalaLocation.h
  ir/location/SourceLocation.cpp
  ir/location/SourceLocation.h
  ir/location/LocationManager.cpp
  ir/location/LocationManager.h
)

if (STATIC_LINKING)
  set(USE_SHARED OFF)
  if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # macOS does not guarantee backwards compatible system calls and therefore
    # discourages statically linked binaries. Instead add -L/usr/lib before the
    # LLVM LDFLAGS to link against the dynamic system libc++ instead of the one
    # from LLVM.
    set(BINDGEN_LINK_FLAG "-L/usr/lib")
  else()
    set(BINDGEN_LINK_FLAG "-static")
  endif()
  llvm_map_components_to_libnames(LLVM_LIBS support core irreader object option profiledata)
else()
  set(LLVM_LIBS LLVM)
endif()
message(STATUS "Using link flag: ${BINDGEN_LINK_FLAG}")

set_target_properties(bindgen
  PROPERTIES
  OUTPUT_NAME scala-native-bindgen
  LINK_FLAGS "${BINDGEN_LINK_FLAG}"
)

target_link_libraries(bindgen
  PRIVATE
  clangFrontend
  clangTooling
  clangSerialization
  clangDriver
  clangParse
  clangSema
  clangAnalysis
  clangEdit
  clangAST
  clangLex
  clangBasic
  ${LLVM_LIBS}
)
