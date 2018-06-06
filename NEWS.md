# Release notes

## master

Improvements:

 - Save results from AST visitor in an intermediate representation. [#6], [#10]
 - Add struct alloc helper. [mrRosset#3], [mrRosset#4]
 - Simplify the build to link against system installed Clang libraries. [mrRosset#2]
 - Use docker-compose to run tests against multiple LLVM versions on Travis. [#8], [#9]
 - Migrate the test suite to Scala and sbt. [#17]
 - Align code with LLVM Coding Standards. [#23]

 [mrRosset#2]: https://github.com/mrRosset/scala-native-bindgen/pull/2
 [mrRosset#3]: https://github.com/mrRosset/scala-native-bindgen/issues/3
 [mrRosset#4]: https://github.com/mrRosset/scala-native-bindgen/pull/4
 [#6]: https://github.com/kornilova-l/scala-native-bindgen/issues/6
 [#8]: https://github.com/kornilova-l/scala-native-bindgen/pull/8
 [#9]: https://github.com/kornilova-l/scala-native-bindgen/issues/9
 [#10]: https://github.com/kornilova-l/scala-native-bindgen/pull/10
 [#17]: https://github.com/kornilova-l/scala-native-bindgen/pull/17

Based on work by [Marius Rosset][@mrRosset] done as part of a project at [EPFL].

 [@mrRosset]: https://github.com/mrRosset
 [EPFL]: https://www.epfl.ch/
