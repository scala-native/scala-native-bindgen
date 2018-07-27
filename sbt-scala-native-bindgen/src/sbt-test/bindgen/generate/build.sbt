name := "test"
organization := "org.scala-native.bindgen.sbt.test"
enablePlugins(ScalaNativeBindgenPlugin)
scalaVersion := "2.11.12"

inConfig(Compile)(
  Def.settings(
    nativeBindgenPath := file(System.getProperty("bindgen.path")),
    nativeBindings := Seq(
      NativeBinding(
        name = "stdlib",
        header = (resourceDirectory in Compile).value / "stdlib.h",
        packageName = Some("org.example.app.stdlib"),
        link = None,
        excludePrefix = Some("__")
      )
    )
  ))

val nativeBindgenCustomTarget = SettingKey[File]("nativeBindgenCustomTarget")
nativeBindgenCustomTarget := baseDirectory.value / "src/main/scala/org/example"

val nativeBindgenCoreBinding =
  SettingKey[NativeBinding]("nativeBindgenCoreBinding")
nativeBindgenCoreBinding := {
  NativeBinding(
    name = "core",
    header = (resourceDirectory in Compile).value / "core.h",
    packageName = Some("org.example.app.core"),
    link = Some("core"),
    excludePrefix = None
  )
}

val StdlibOutput =
  """package org.example.app.stdlib
    |
    |import scala.scalanative._
    |import scala.scalanative.native._
    |
    |@native.extern
    |object stdlib {
    |  def access(path: native.CString, mode: native.CInt): native.CInt = native.extern
    |  def read(fildes: native.CInt, buf: native.Ptr[Byte], nbyte: native.CInt): native.CInt = native.extern
    |  def printf(format: native.CString, varArgs: native.CVararg*): native.CInt = native.extern
    |}
  """.stripMargin

def assertFileContent(file: File, expected: String): Unit = {
  val actual = IO.read(file).trim
  if (actual != expected.trim) {
    println(s"== [ actual ${file.getName} ] ========")
    println(actual)
    println(s"== [ expected ${file.getName} ] ========")
    println(expected.trim)
  }
  assert(actual == expected.trim)
}

TaskKey[Unit]("checkSingle") := {
  val files = (nativeBindgen in Compile).value
  assert(files.forall(_.exists()))
  assert(files.length == 1)
  assertFileContent(files.head, StdlibOutput)
}

TaskKey[Unit]("checkMultiple") := {
  val files = (nativeBindgen in Compile).value
  assert(files.forall(_.exists()))
  assert(files.length == 2)

  assertFileContent(files(0), StdlibOutput)

  val CoreOutput =
    """package org.example.app.core
      |
      |import scala.scalanative._
      |import scala.scalanative.native._
      |
      |@native.link("core")
      |@native.extern
      |object core {
      |  def count_words(text: native.CString): native.CInt = native.extern
      |  def __not_excluded(): Unit = native.extern
      |}
    """.stripMargin

  assertFileContent(files(1), CoreOutput)
}
