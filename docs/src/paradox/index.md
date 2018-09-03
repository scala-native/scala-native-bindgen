# Scala Native Bindgen

Scala Native Bindgen is a binding generator for Scala Native that uses [Clang] to
parse C header files and generates Scala binding definitions.

## Features

* possibility to reuse types from existing bindings.
* type casts that make recursive structs be valid Scala Native structs.
* implicit classes for structs and unions that make fields access easier.
* implicit classes that add setters and getters to structs with more than 22 fields (such structs in Scala
  Native are represented as arrays of bytes).
* literal defines embedding `#define MY_CONSTANT 42` → `val MY_CONSTANT: native.CInt = 42`.
* read-only bindings for extern variables (such variables cannot be updated due to Scala Native limitation).
* declarations filtering by prefix.

## License

This project is distributed under the Scala license.
@github[See LICENSE.txt for details](/LICENSE.txt)

 [Clang]: https://clang.llvm.org/

## Table of Contents

@@ toc

@@@ index

* [sbt](sbt.md)
* [cli](cli.md)
* [usage](using-bindings.md)
* [configuration](configuration.md)
* [Limitations](limitations.md)
* [bindings](bindings/index.md)
* [Contributor's Guide](contrib/index.md)

@@@
