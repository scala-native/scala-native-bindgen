# Scala Native Binding generator

This tool generate Scala Native bindings from C headers. It's build upon clang and Libtooling and thus respect the conventions of clang-tools.

## Usage

Calling the tool is pretty easy, you need to specify the file(s) and the name of the created bindings.

`./scalaBindgen /usr/include/uv.h -name uv --`

Running the previous command wild also yield warnings along with the translation. To keep only the bindings please redirect the output to a file like this:

`./scalaBindgen /usr/include/uv.h -name uv -- > uv.scala`

## Building

Building this tool require building LLVM and Clang. This can take hours. Please consider using pre-built binaries.

* Follow the [building instructions](http://clang.llvm.org/docs/LibASTMatchersTutorial.html#step-0-obtaining-clang) for Clang (Only step 0)
* Go to `llvm/tools/clang/tools/extra`
* Clone this repository `git clone https://github.com/mrRosset/scalaBindgen.git`
* Add `add_subdirectory(scala-bindgen)` to the CMakeLists.txt in the extra folder
* Re-run the llvm/clang compilation

