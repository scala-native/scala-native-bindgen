package org.scalanative.bindgen.sbt

import sbt._
import sbt.Keys._

import java.nio.file.Files
import java.nio.file.attribute.{PosixFileAttributeView, PosixFilePermission}

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
 *  - `nativeBindgenPath`: Path to the `scala-native-bindgen` executable.
 *  - `nativeBindgenHeader`: The C header file to read.
 *
 *  - `nativeBindgenPackage`: Package of the enclosing object.
 *    No package by default.
 *
 *  - `name in nativeBindgen`: Name of the enclosing object.
 *
 *  - `version in nativeBindgen`: Version of the `scala-native-bindgen`
 *    to use when automatically downloading the executable.
 *
 *  - `nativeBindgenLink`: Name of library to be linked.
 *
 *  - `nativeBindgen`: Generate Scala Native bindings.
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
    val ScalaNativeBindgen = config("scala-native-bindgen").hide
    val nativeBindgenPath =
      taskKey[File]("Path to the scala-native-bindgen executable")
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
    inConfig(ScalaNativeBindgen)(Defaults.configSettings) ++
    nativeBindgenScopedSettings(Compile) ++
    Def.settings(
      ivyConfigurations += ScalaNativeBindgen,
      version in nativeBindgen := BuildInfo.version,
      libraryDependencies ++= {
        artifactName.map { name =>
          val bindgenVersion = (version in nativeBindgen).value
          val url =
            s"${BuildInfo.projectUrl}/releases/download/v$bindgenVersion/$name"

          BuildInfo.organization % name % bindgenVersion % ScalaNativeBindgen from (url)
        }.toSeq
      },
      nativeBindgenPath := {
        val scalaNativeBindgenUpdate = (update in ScalaNativeBindgen).value

        val artifactFile = artifactName match {
          case None =>
            sys.error(
              "No downloadable binaries available for your OS, " +
                "please provide path via `nativeBindgenPath`")
          case Some(name) =>
            scalaNativeBindgenUpdate
              .select(artifact = artifactFilter(name = name))
              .head
        }

        // Set the executable bit on the expected path to fail if it doesn't exist
        for (view <- Option(
                Files.getFileAttributeView(artifactFile.toPath,
                                          classOf[PosixFileAttributeView]))) {
          val permissions = view.readAttributes.permissions
          if (permissions.add(PosixFilePermission.OWNER_EXECUTE))
            view.setPermissions(permissions)
        }

        artifactFile
      }
    )

  private implicit class BindgenOps(val bindgen: Bindgen) extends AnyVal {
    def maybe[T](opt: Option[T], f: Bindgen => T => Bindgen): Bindgen =
      opt match {
        case None        => bindgen
        case Some(value) => f(bindgen)(value)
      }
  }

  private val artifactName =
    Option(System.getProperty("os.name")).collect {
      case "Mac OS X" => "scala-native-bindgen-darwin"
      case "Linux"    => "scala-native-bindgen-linux"
    }

  def nativeBindgenScopedSettings(conf: Configuration): Seq[Setting[_]] =
    inConfig(conf)(
      Def.settings(
        nativeBindgenHeader := {
          sys.error("nativeBindgenHeader not configured")
        },
        nativeBindgenPackage := None,
        nativeBindgenExclude := None,
        resourceDirectories in nativeBindgen := resourceDirectories.value,
        sourceGenerators += Def.task { Seq(nativeBindgen.value) },
        name in nativeBindgen := "ScalaNativeBindgen",
        nativeBindgen := {
          val output = sourceManaged.value / "sbt-scala-native-bindgen" / "ScalaNativeBindgen.scala"

          Bindgen()
            .bindgenExecutable(nativeBindgenPath.value)
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
