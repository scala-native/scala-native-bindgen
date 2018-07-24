name := "test"
organization := "org.scala-native.bindgen.sbt.test"
enablePlugins(ScalaNativeBindgenPlugin)
scalaVersion := "2.11.12"

inConfig(Compile)(
  Def.settings(
    nativeBindgenPath := file(System.getProperty("bindgen.path")),
    nativeBindgenHeader := (resourceDirectory in Compile).value / "header.h",
    nativeBindgenPackage := Some("org.example.app"),
    nativeBindgenLink := Some("app"),
    nativeBindgenExclude := Some("__"),
    name in nativeBindgen := "AppAPI"
  ))

TaskKey[Unit]("check") := {
  val file = (nativeBindgen in Compile).value
  val expected =
    """package org.example.app
      |
      |import scala.scalanative._
      |import scala.scalanative.native._
      |
      |@native.link("app")
      |@native.extern
      |object AppAPI {
      |  def access(path: native.CString, mode: native.CInt): native.CInt = native.extern
      |  def read(fildes: native.CInt, buf: native.Ptr[Byte], nbyte: native.CInt): native.CInt = native.extern
      |  def printf(format: native.CString, varArgs: native.CVararg*): native.CInt = native.extern
      |}
    """.stripMargin

  assert(file.exists)
  assert(IO.read(file).trim == expected.trim)
}
