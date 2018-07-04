# Building with `CMake`

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

## Testing

The tests assume that the above instructions for building `scala-native-bindgen` from source
has been followed.

```sh
cd tests
sbt test
```

 [CMake]: https://cmake.org/
 [LLVM]: https://llvm.org/
 [Clang]: https://clang.llvm.org/
 [Scala Native setup guide]: http://www.scala-native.org/en/latest/user/setup.html
