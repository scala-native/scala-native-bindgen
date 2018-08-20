# Generating Bindings with sbt

To add the sbt plugin to your project add the following lines in `project/plugins.sbt`:

@@snip [sbt-plugin] (../../../project/plugins.sbt) { #sbt-plugin-example }

Replacing `VERSION` with @var[project.version] to use the latest version.

Next configure the plugin using the `nativeBindings` setting scoped to either `Compile` or `Test`. The `NativeBinding` type to configure each binding that should be generated. 

@@@ note

By default the `scala-native-bindgen` executable is downloaded automatically for supported platforms. Set `version in nativeBindgen` (unscoped) to configure the version of the `scala-native-bindgen` to use if you want a version different from the version of the sbt plugin.

In case your platform is not supported, you must compile `scala-native-bindgen` yourself and configure the path to the executable using `nativeBindgenPath`, e.g.:

```sbt
nativeBindgenPath := file("/path/to/scala-native-bindgen")
```

@@@

Example settings:

@@snip [sbt] (../../../sbt-scala-native-bindgen/src/sbt-test/bindgen/generate/build.sbt) { #example }

Given that the `stdlib.h` header file contains:

@@snip [sbt] (../../../sbt-scala-native-bindgen/src/sbt-test/bindgen/generate/src/main/resources/stdlib.h)

Running `nativeBindgen` will generate a file named `target/scala-2.11/src_managed/main/sbt-scala-native-bindgen/stdlib.scala` containing the following lines:

@@snip [sbt] (../../../sbt-scala-native-bindgen/src/sbt-test/bindgen/generate/expected/stdlib.scala)