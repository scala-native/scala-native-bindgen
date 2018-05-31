# Scala Native Binding generator

[![Build Status](https://travis-ci.com/kornilova-l/scala-native-bindgen.svg?branch=master)](https://travis-ci.com/kornilova-l/scala-native-bindgen)

This tool generate Scala Native bindings from C headers. It's built upon clang and Libtooling and thus respect the conventions of clang-tools.

## Usage

Calling the tool is pretty easy, you need to specify the file(s) and the name of the created bindings.

`./scalaBindgen /usr/include/uv.h -name uv --`

Running the previous command wild also yield warnings along with the translation. To keep only the bindings please redirect the output to a file like this:

`./scalaBindgen /usr/include/uv.h -name uv -- > uv.scala`

## Building

Building this tool requires LLVM and Clang. Ensure that `llvm-config` is in your path.

```sh
# Validate LLVM installation
llvm-config --version --cmakedir --cxxflags --ldflags

mkdir -p target
cd target
cmake ..
make
./scalaBindgen /usr/include/ctype.h -name ctype
```

## License

This project is distributed under the Scala license.
[See LICENSE.txt for details](LICENSE.txt)
