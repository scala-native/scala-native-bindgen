# Guide for contributors

Scala Native Bindgen follows Scala Native's [contributing guidelines].
Larger contributions should always begin by creating an issue to
ensure that it is properly scoped.

Important to note is that all contributors must have signed the [Scala CLA].

 [contributing guidelines]: http://www.scala-native.org/en/latest/contrib/contributing.html
 [Scala CLA]: https://www.lightbend.com/contribute/cla/scala

## Developer Workflow

Build the `scalaBindgen` tool:

```sh
mkdir target
cd target
cmake ..
make

# Alternatively rerun on change
watchman-make -p '*.cpp' '*.h' --run 'make -C target'
```

In another terminal, run the test suite:

```sh
cd tests
sbt ~test
```

## Coding Guidelines

The C++ tool is built on Clang and Libtooling and should respect the conventions of
LLVM and Clang tools. The code itself should adhere to the [LLVM Coding Standards],
specifically:

 - For code generation and error reporting use `llvm::outs()` and `llvm::errs()`.
 - [Use `\n` instead of `std::endl`](https://llvm.org/docs/CodingStandards.html#avoid-std-endl)
   and remember to flush when reporting errors.

 [LLVM Coding Standards]: https://llvm.org/docs/CodingStandards.html