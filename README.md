# Scala Native Binding generator

[![Build Status](https://travis-ci.com/kornilova-l/scala-native-bindgen.svg?branch=master)](https://travis-ci.com/kornilova-l/scala-native-bindgen)

This tool generates Scala Native bindings from C headers. It's built upon clang and Libtooling and thus respects the conventions of clang-tools.

[Documentation](https://kornilova-l.github.io/scala-native-bindgen/)

## Releasing

To release version `x.y.z` run:

    > sbt -Dproject.version=x.y.z release

Then build the `scala-native-bindgen` executable for both macOS and
Linux and upload them to the GitHub release page with the suffix
`-darwin` and `-linux`, respectively.

## License

This project is distributed under the Scala license.
[See LICENSE.txt for details](LICENSE.txt)
