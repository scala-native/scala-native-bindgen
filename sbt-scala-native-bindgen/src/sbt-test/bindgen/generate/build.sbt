name := "test"
organization := "org.scala-native.bindgen.sbt.test"
//#example
enablePlugins(ScalaNativeBindgenPlugin)

//#example
scalaVersion := "2.11.12"
nativeBindgenPath := Some(file(System.getProperty("bindgen.path")))

//#example
inConfig(Compile)(
  Def.settings(
    nativeBindings += {
      //#sbt-exclude-prefix
      NativeBinding(resourceDirectory.value / "stdlib.h")
      //#sbt-exclude-prefix
        .name("stdlib")
        .packageName("org.example.app.stdlib")
        //#sbt-exclude-prefix
        .excludePrefix("__")
      //#sbt-exclude-prefix
    }
  ))
//#example

val nativeBindgenCustomTarget = SettingKey[File]("nativeBindgenCustomTarget")
nativeBindgenCustomTarget := baseDirectory.value / "src/main/scala/org/example"

val nativeBindgenCoreBinding =
  SettingKey[NativeBinding]("nativeBindgenCoreBinding")
nativeBindgenCoreBinding := {
  NativeBinding((resourceDirectory in Compile).value / "core.h")
    .packageName("org.example.app.core")
    .link("core")
}