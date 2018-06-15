# Release notes

## master

Improvements:

 - Save results from AST visitor in an intermediate representation. [#6], [#10]
 - Add struct alloc helper. [mrRosset#3], [mrRosset#4]
 - Simplify the build to link against system installed Clang libraries. [mrRosset#2]
 - Use docker-compose to run tests against multiple LLVM versions on Travis. [#8], [#9]
 - Migrate the test suite to Scala and sbt. [#17]
 - Align code with LLVM Coding Standards. [#23]
 - Filter functions and typedefs by prefix. [#25]
 - Ensure that names of library member do not interfere with Scala names. [#32]
 - Support the case when enum values are explicitly set. [#37]
 - Set correct underlying type of enums. [#38]
 - Use automatic source formatting. [#41]
 - Fix name of variadic argument. [#42]
 - Add command line option for package name. [#43]
 - Warn about packed structs. [#44]

 [mrRosset#2]: https://github.com/mrRosset/scala-native-bindgen/pull/2
 [mrRosset#3]: https://github.com/mrRosset/scala-native-bindgen/issues/3
 [mrRosset#4]: https://github.com/mrRosset/scala-native-bindgen/pull/4
 [#6]: https://github.com/kornilova-l/scala-native-bindgen/issues/6
 [#8]: https://github.com/kornilova-l/scala-native-bindgen/pull/8
 [#9]: https://github.com/kornilova-l/scala-native-bindgen/issues/9
 [#10]: https://github.com/kornilova-l/scala-native-bindgen/pull/10
 [#17]: https://github.com/kornilova-l/scala-native-bindgen/pull/17
 [#23]: https://github.com/kornilova-l/scala-native-bindgen/pull/23
 [#25]: https://github.com/kornilova-l/scala-native-bindgen/pull/25
 [#32]: https://github.com/kornilova-l/scala-native-bindgen/issues/32
 [#37]: https://github.com/kornilova-l/scala-native-bindgen/pull/37
 [#38]: https://github.com/kornilova-l/scala-native-bindgen/pull/38
 [#41]: https://github.com/kornilova-l/scala-native-bindgen/pull/41
 [#42]: https://github.com/kornilova-l/scala-native-bindgen/pull/42
 [#43]: https://github.com/kornilova-l/scala-native-bindgen/pull/43
 [#44]: https://github.com/kornilova-l/scala-native-bindgen/pull/44
 [#47]: https://github.com/kornilova-l/scala-native-bindgen/pull/47

Based on work by [Marius Rosset][@mrRosset] done as part of a project at [EPFL].

 [@mrRosset]: https://github.com/mrRosset
 [EPFL]: https://www.epfl.ch/
