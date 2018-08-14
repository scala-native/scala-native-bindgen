name := "test"
organization := "org.scala-native.bindgen.sbt.test"
//#example
enablePlugins(ScalaNativeBindgenPlugin)

//#example
scalaVersion := "2.11.12"

//#example
inConfig(Compile)(
  Def.settings(
    //#example
    nativeBindgenPath := file(System.getProperty("bindgen.path")),
    //#example
    nativeBindings += {
      NativeBinding((resourceDirectory in Compile).value / "stdlib.h")
        .name("stdlib")
        .packageName("org.example.app.stdlib")
        .excludePrefix("__")
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