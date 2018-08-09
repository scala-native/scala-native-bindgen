# Scala Native Binding generator

[![Build Status](https://travis-ci.com/kornilova-l/scala-native-bindgen.svg?branch=master)](https://travis-ci.com/kornilova-l/scala-native-bindgen)

This tool generates Scala Native bindings from C headers. It's built upon clang and Libtooling and thus respects the conventions of clang-tools.

[Documentation](https://kornilova-l.github.io/scala-native-bindgen/)

## Releasing

First build the `scala-native-bindgen` executable for both macOS and
Linux:

    > scripts/prepare-release.sh

You should now have `scala-native-bindgen-linux` and
`scala-native-bindgen-darwin` if you ran the script on a macOS machine.

Then release version `x.y.z` by running:

    > sbt -Dproject.version=x.y.z release

Finally, upload the `scala-native-bindgen-linux` and
`scala-native-bindgen-darwin` executables to the release page at:
<https://github.com/kornilova-l/scala-native-bindgen/releases/tag/vx.y.z>

## License

This project is distributed under the Scala license.
[See LICENSE.txt for details](LICENSE.txt)
