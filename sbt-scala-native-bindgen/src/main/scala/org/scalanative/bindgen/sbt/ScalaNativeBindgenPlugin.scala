package org.scalanative.bindgen.sbt

import sbt._
import sbt.Keys._

import org.scalanative.bindgen.Bindgen

/**
 * Generate Scala bindings from C headers.
 *
 * == Usage ==
 *
 * This plugin must be explicitly enabled. To enable it add the following line
 * to your `.sbt` file:
 * {{{
 * enablePlugins(ScalaNativeBindgenPlugin)
 * }}}
 *
 * By default, the plugin reads the configured header file and generates
 * a single Scala source file in the managed source directory.
 *
 * See the [[https://github.com/kornilova-l/scala-native-bindgen/tree/master/sbt-scala-native-bindgen/src/sbt-test/bindgen/generate example project]].
 *
 * == Configuration ==
 *
 * Keys are defined in [[ScalaNativeBindgenPlugin.autoImport]].
 *
 *  - `nativeBindgenHeader`: The C header file to read.
 *
 *  - `nativeBindgenPackage`: Package of the enclosing object.
 *    No package by default.
 *
 *  - `name in nativeBindgen`: Name of the enclosing object.
 *
 * @example
 * {{{
 * nativeBindgenHeader in Compile := file("/usr/include/ctype.h")
 * nativeBindgenPackage in Compile := Some("org.example.app")
 * name in (Compile, nativeBindgen) := "ctype"
 * }}}
 */
object ScalaNativeBindgenPlugin extends AutoPlugin {

  object autoImport {
    val nativeBindgenPath =
      taskKey[Option[File]]("Path to the scala-native-bindgen executable")
    val nativeBindgenHeader = taskKey[File]("C header file")
    val nativeBindgenPackage =
      settingKey[Option[String]]("Package for the generated code")
    val nativeBindgenLink =
      settingKey[Option[String]]("Name of library to be linked")
    val nativeBindgenExclude = settingKey[Option[String]]("Exclude prefix")
    val nativeBindgen        = taskKey[File]("Generate Scala Native bindings")
  }
  import autoImport._

  override def requires = plugins.JvmPlugin

  override def projectSettings: Seq[Setting[_]] =
    nativeBindgenScopedSettings(Compile)

  private implicit class BindgenOps(val bindgen: Bindgen) extends AnyVal {
    def maybe[T](opt: Option[T], f: Bindgen => T => Bindgen): Bindgen =
      opt match {
        case None        => bindgen
        case Some(value) => f(bindgen)(value)
      }
  }

  def nativeBindgenScopedSettings(conf: Configuration): Seq[Setting[_]] =
    inConfig(conf)(
      Seq(
        nativeBindgenHeader := {
          sys.error("nativeBindgenHeader not configured")
        },
        nativeBindgenPath := None,
        nativeBindgenPackage := None,
        nativeBindgenExclude := None,
        resourceDirectories in nativeBindgen := resourceDirectories.value,
        sourceGenerators += Def.task { Seq(nativeBindgen.value) },
        name in nativeBindgen := "ScalaNativeBindgen",
        nativeBindgen := {
          val output = sourceManaged.value / "sbt-scala-native-bindgen" / "nativeBindgen.scala"

          Bindgen()
            .bindgenExecutable(nativeBindgenPath.value.get)
            .header(nativeBindgenHeader.value)
            .name((name in nativeBindgen).value)
            .maybe(nativeBindgenLink.value, _.link)
            .maybe(nativeBindgenPackage.value, _.packageName)
            .maybe(nativeBindgenExclude.value, _.excludePrefix)
            .generate()
            .writeToFile(output)

          output
        }
      ))
}
