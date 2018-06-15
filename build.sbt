import scala.scalanative.sbtplugin.ScalaNativePluginInternal.nativeWorkdir
import scala.sys.process._
import java.nio.file.Path

inThisBuild(
  Def.settings(
    organization := "org.scalanative.bindgen",
    version := "0.1-SNAPSHOT",
    scalaVersion := "2.11.12",
    scalacOptions ++= Seq(
      "-deprecation",
      "-unchecked",
      "-feature",
      "-encoding",
      "utf8"
    )
  ))

val tests = project
  .in(file("tests"))
  .aggregate(samples)
  .settings(
    fork in Test := true,
    javaOptions in Test += {
      val rootDir = (ThisBuild / baseDirectory).value
      s"-Dbindgen.path=$rootDir/bindgen/target/scala-native-bindgen"
    },
    watchSources += WatchSource(
      baseDirectory.value / "tests" / "samples",
      "*.h" || "*.scala",
      NothingFilter
    ),
    libraryDependencies += "org.scalatest" %% "scalatest" % "3.0.5" % Test
  )

lazy val samples = project
  .in(file("tests/samples"))
  .enablePlugins(ScalaNativePlugin)
  .settings(
    libraryDependencies += "com.lihaoyi" %%% "utest" % "0.6.3" % "test",
    testFrameworks += new TestFramework("utest.runner.Framework"),
    nativeLinkStubs := true,
    Test / nativeLinkingOptions ++= {
      val rootDir = (ThisBuild / baseDirectory).value.getAbsoluteFile
      val cwd     = (Test / target).value.getAbsoluteFile / "bindgen"
      val linker  = rootDir / "scripts" / "linker.sh"
      Seq(s"-L$cwd", s"-fuse-ld=$linker")
    },
    Test / compile := {
      val log            = streams.value.log
      val cwd            = (Test / target).value / "bindgen"
      val compileOptions = (Test / nativeCompileOptions).value
      val cpaths         = (baseDirectory.value.getAbsoluteFile * "*.c").get
      val clangPath      = nativeClang.value.toPath.toAbsolutePath.toString

      cwd.mkdirs()

      def abs(path: File): String =
        path.getAbsolutePath

      def run(command: Seq[String]): Int = {
        log.info("Running " + command.mkString(" "))
        Process(command, cwd) ! log
      }

      val opaths = cpaths.map { cpath =>
        val opath = abs(cwd / s"${cpath.getName}.o")
        val command = Seq(clangPath) ++ compileOptions ++ Seq("-c",
                                                              abs(cpath),
                                                              "-o",
                                                              opath)

        if (run(command) != 0) {
          sys.error(s"Failed to compile $cpath")
        }
        opath
      }

      val archivePath = cwd / "libbindgentests.a"
      val archive     = Seq("ar", "cr", abs(archivePath)) ++ opaths
      if (run(archive) != 0) {
        sys.error(s"Failed to create archive $archivePath")
      }

      (Test / compile).value
    }
  )
