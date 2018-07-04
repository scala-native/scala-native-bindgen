# Obtaining bindgen

There are 2 ways to obtain bindgen:
 * [Use docker container](#docker-container) 
 * [Build binary with CMake](#building-with-cmake)
 * [Build binary with docker-compose](#building-with-docker-compose)

## Docker container

This option requires [Docker].

Download docker image with the binary:

```sh
docker pull scalabindgen/scala-native-bindgen
```

Mount directories with required header files and run bindgen:

```sh
docker run -v "$(pwd)":/src -v /usr/include:/usr/include \
  --rm scalabindgen/scala-native-bindgen \
  relative/path/to/my_header.h --name my_header --
```

The docker image does not contain standard headers so it is important to
mount all system include directories that are used by the header file
passed to `scala-native-bindgen`. See the [docker-bindgen.sh] script for
how to wrap the dockerized program. The `$CWD` of the container is
`/src` which should be mounted from `$(pwd)` in case relative paths are
used.

Note, the `scalabindgen/scala-native-bindgen:latest` image is updated on
each merge to the `master` branch.

 [Docker]: https://www.docker.com/
 [docker-bindgen.sh]: scripts/docker-bindgen.sh

## Building with `CMake`

Building `scala-native-bindgen` requires the following tools and libraries:

 - [CMake] version 3.9 or higher
 - [LLVM] and [Clang] version 5.0 or 6.0.

```sh
# On apt-based systems
apt install cmake clang-$LLVM_VERSION libclang-$LLVM_VERSION-dev llvm-$LLVM_VERSION-dev

# With `brew`
brew install cmake llvm@6
```

To run the tests you also need the required Scala Native libraries.
See the [Scala Native setup guide] for instructions on installing the dependencies.

```sh
mkdir -p bindgen/target
cd bindgen/target
cmake ..
make
./scala-native-bindgen --name ctype /usr/include/ctype.h --
```

To build a statically linked executable pass `-DSTATIC_LINKING=ON` when invoking `cmake`:

```sh
cmake -DSTATIC_LINKING=ON ..
```

Additional compiler and linker flags may be passed as environment variable sor their CMake
equivalent, e.g. to compile with debug symbols the following are the same:

```sh
cmake -DCMAKE_CXX_FLAGS=-g ..
CXXFLAGS=-g cmake ..
```

## Building with `docker-compose`

Alternatively, you can use [docker-compose] to build and test the program:

```sh
# Build the docker image with LLVM version 6.0.
docker-compose build ubuntu-18.04-llvm-6.0
# Build the bindgen tool and run the tests.
docker-compose run --rm ubuntu-18.04-llvm-6.0 ./script/test.sh
# Run the bindgen tool inside the container.
docker-compose run --rm ubuntu-18.04-llvm-6.0 \
    bindgen/target/scala-native-bindgen --name union tests/samples/Union.h --
```

 [CMake]: https://cmake.org/
 [LLVM]: https://llvm.org/
 [Clang]: https://clang.llvm.org/
 [Scala Native setup guide]: http://www.scala-native.org/en/latest/user/setup.html
 [docker-compose]: https://docs.docker.com/compose/

## Testing

The tests assume that the above instructions for building `scala-native-bindgen` from source
has been followed.

```sh
cd tests
sbt test
```