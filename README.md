# Scala Native Binding generator

[![Build Status](https://travis-ci.com/kornilova-l/scala-native-bindgen.svg?branch=master)](https://travis-ci.com/kornilova-l/scala-native-bindgen)

This tool generates Scala Native bindings from C headers. It's built upon clang and Libtooling and thus respects the conventions of clang-tools.

## Usage

Calling the tool is pretty easy, you need to specify the file(s) and the name of the created bindings.

`./scala-native-bindgen /usr/include/uv.h -name uv --`

Running the previous command wild also yield warnings along with the translation. To keep only the bindings please redirect the output to a file like this:

`./scala-native-bindgen /usr/include/uv.h -name uv -- > uv.scala`

## Building

Building this tool requires [CMake], [LLVM] and [Clang]. See the [Scala
Native setup guide] for instructions on installing the dependencies.

```sh
mkdir -p bindgen/target
cd bindgen/target
cmake ..
make
./scala-native-bindgen /usr/include/ctype.h -name ctype --
```

Alternatively, you can use [docker-compose] to build and test the program:

```sh
# Build the docker image with LLVM version 6.0.
docker-compose build ubuntu-18.04-llvm-6.0
# Build the bindgen tool and run the tests.
docker-compose run --rm ubuntu-18.04-llvm-6.0
# Run the bindgen tool inside the container.
docker-compose run --rm ubuntu-18.04-llvm-6.0 bindgen/target/scala-native-bindgen -name union tests/samples/Union.h --
```

 [CMake]: https://cmake.org/
 [LLVM]: https://llvm.org/
 [Clang]: https://clang.llvm.org/
 [Scala Native setup guide]: http://www.scala-native.org/en/latest/user/setup.html
 [docker-compose]: https://docs.docker.com/compose/

## Testing

The tests assume that the above instructions for building has been
followed.

```sh
cd tests
sbt test
```

## License

This project is distributed under the Scala license.
[See LICENSE.txt for details](LICENSE.txt)
