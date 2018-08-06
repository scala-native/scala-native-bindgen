# Using the sbt plugin

To add the sbt plugin to your project add the following lines in `project/plugins.sbt`:

@@@ vars
```sbt
addSbtPlugin("org.scala-native.bindgen" % "sbt-scala-native-bindgen" % "$project.version$")

resolvers += Resolver.bintrayIvyRepo("scala-native-bindgen", "sbt-plugin")
resolvers += Resolver.bintrayRepo("scala-native-bindgen", "maven")
```
@@@

Next configure the plugin using the `nativeBindings` setting scoped to either `Compile` or `Test`. The `NativeBinding` type to configure each binding that should be generated. 

@@@ note

By default the `scala-native-bindgen` executable is downloaded automatically for supported platforms. Set `version in nativeBindgen` (unscoped) to configure the version of the `scala-native-bindgen` to use if you want a version different from the version of the sbt plugin.

In case your platform is not supported, you must compile `scala-native-bindgen` yourself and configure the path to the executable using `nativeBindgenPath`, e.g.:

```sbt
nativeBindgenPath := file("/path/to/scala-native-bindgen")
```

@@@

Example settings:

```sbt
enablePlugins(ScalaNativeBindgenPlugin)
inConfig(Compile)(
  Def.settings(
    nativeBindings += {
      NativeBinding((resourceDirectory in Compile).value / "header.h")
        .name("MyLib")
        .packageName("org.example.mylib")
        .link("mylib"), // Will pass `-lmylib` to the linker
        .excludePrefix("__")
      }
  ))
```

Running `nativeBindgen` will generate a file named `target/scala-2.x/src_managed/main/sbt-scala-native-bindgen/MyLib.scala` containing something along the following lines:

```scala
package org.example.mylib

import scala.scalanative._
import scala.scalanative.native._

@native.link("mylib")
@native.extern
object MyLib {
  // ... left out for brevity ...
}
```
