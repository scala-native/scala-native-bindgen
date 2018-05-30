# Scala Native Binding generator

[![Build Status](https://travis-ci.com/kornilova-l/scala-native-bindgen.svg?branch=master)](https://travis-ci.com/kornilova-l/scala-native-bindgen)

This tool generate Scala Native bindings from C headers. It's built upon clang and Libtooling and thus respect the conventions of clang-tools.

## Usage

Calling the tool is pretty easy, you need to specify the file(s) and the name of the created bindings.

`./scalaBindgen /usr/include/uv.h -name uv --`

Running the previous command wild also yield warnings along with the translation. To keep only the bindings please redirect the output to a file like this:

`./scalaBindgen /usr/include/uv.h -name uv -- > uv.scala`

## Building

Building this tool requires [CMake], [LLVM] and [Clang]. See the [Scala
Native setup guide] for instructions on installing the dependencies.

```sh
mkdir -p target
cd target
cmake ..
make
./scalaBindgen /usr/include/ctype.h -name ctype --
```

 [CMake]: https://cmake.org/
 [LLVM]: https://llvm.org/
 [Clang]: https://clang.llvm.org/
 [Scala Native setup guide]: http://www.scala-native.org/en/latest/user/setup.html

## Testing

The tests assumes that the above instructions for building has been
followed.

```sh
cd tests
sbt test
```

## License

This project is distributed under the Scala license.
[See LICENSE.txt for details](LICENSE.txt)
