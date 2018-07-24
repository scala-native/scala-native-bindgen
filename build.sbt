import scala.sys.process._

addCommandAlias("verify", "; ^test:compile ; ^test ; ^scripted ; docs/makeSite")

val Versions = new {
  val scala210 = "2.10.6"
  val scala211 = "2.11.12"
  val scala212 = "2.12.6"
  val sbt013   = "0.13.17"
  val sbt1     = "1.1.6"
}

inThisBuild(
  Def.settings(
    organization := "org.scala-native.bindgen",
    licenses := Seq(
      "BSD 3-Clause" -> url("https://www.scala-lang.org/license/")),
    homepage := Some(url("https://kornilova-l.github.io/scala-native-bindgen")),
    scalacOptions ++= Seq(
      "-deprecation",
      "-unchecked",
      "-feature",
      "-encoding",
      "utf8"
    ),
    scmInfo := Some(
      ScmInfo(url("https://github.com/kornilova-l/scala-native-bindgen"),
              "scm:git:git@github.com:kornilova-l/scala-native-bindgen.git")),
    developers := List(
      Developer(
        id = "kornilova-l",
        name = "Liudmila Kornilova",
        email = "kornilova-l@users.noreply.github.com",
        url = url("https://github.com/kornilova-l")
      ),
      Developer(
        id = "jonas",
        name = "Jonas Fonseca",
        email = "jonas@users.noreply.github.com",
        url = url("https://github.com/jonas")
      )
    )
  ))

val root = project("scala-native-bindgen")
  .in(file("."))
  .aggregate(
    tests,
    samples,
    tools,
    sbtPlugin,
    docs
  )
  .enablePlugins(ReleasePlugin)
  .settings(
    publish / skip := true,
    releaseCrossBuild := false,
    releaseVersionFile := target.value / "unused-version.sbt",
    releaseProcess := {
      import ReleaseTransformations._
      Seq[ReleaseStep](
        checkSnapshotDependencies,
        setReleaseVersions(version.value),
        runClean,
        releaseStepCommandAndRemaining("verify"),
        setReleaseVersion,
        tagRelease,
        releaseStepCommandAndRemaining("^publish"),
        pushChanges,
        releaseStepTask(docs / ghpagesPushSite)
      )
    }
  )

lazy val tests = project("tests")
  .dependsOn(tools)
  .settings(
    publish / skip := true,
    Test / fork := true,
    Test / javaOptions += {
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

lazy val samples = project("samples")
  .in(file("tests/samples"))
  .enablePlugins(ScalaNativePlugin)
  .settings(
    publish / skip := true,
    scalaVersion := Versions.scala211,
    libraryDependencies += "com.lihaoyi" %%% "utest" % "0.6.3" % "test",
    testFrameworks += new TestFramework("utest.runner.Framework"),
    nativeLinkStubs := true,
    Test / nativeLinkingOptions += {
      Seq("-L", (Test / target).value.getAbsoluteFile / "bindgen").mkString
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

lazy val tools = project("tools")

lazy val sbtPlugin = project("sbt-scala-native-bindgen", ScriptedPlugin)
  .dependsOn(tools)
  .enablePlugins(BuildInfoPlugin)
  .settings(
    Keys.sbtPlugin := true,
    scriptedLaunchOpts += s"-Dplugin.version=${version.value}",
    scriptedLaunchOpts += {
      val rootDir = (ThisBuild / baseDirectory).value
      s"-Dbindgen.path=$rootDir/bindgen/target/scala-native-bindgen"
    },
    buildInfoPackage := "org.scalanative.bindgen.sbt",
    buildInfoKeys := Seq[BuildInfoKey](
      version,
      organization,
      BuildInfoKey.map(scmInfo) {
        case (k, v) => "projectUrl" -> v.get.browseUrl
      }
    ),
    publishLocal := publishLocal.dependsOn(tools / publishLocal).value
  )

lazy val docs = project("docs")
  .enablePlugins(GhpagesPlugin, ParadoxSitePlugin, ParadoxMaterialThemePlugin)
  .settings(
    publish / skip := true,
    Paradox / paradoxProperties ++= Map(
      "github.base_url" -> scmInfo.value.get.browseUrl.toString
    ),
    ParadoxMaterialThemePlugin.paradoxMaterialThemeSettings(Paradox),
    Paradox / paradoxMaterialTheme := {
      (Paradox / paradoxMaterialTheme).value
        .withRepository(scmInfo.value.get.browseUrl.toURI)
        .withColor("indigo", "indigo")
    }
  )

def project(name: String, plugged: AutoPlugin*) = {
  val unplugged = Seq(ScriptedPlugin).filterNot(plugged.toSet)

  Project(id = name, base = file(name))
    .disablePlugins(unplugged: _*)
    .enablePlugins(GitPlugin)
    .enablePlugins(GitVersioning)
    .settings(
      versionWithGit,
      git.useGitDescribe := true,
      git.remoteRepo := scmInfo.value.get.connection.replace("scm:git:", ""),
      crossSbtVersions := List(Versions.sbt013, Versions.sbt1),
      scalaVersion := {
        (pluginCrossBuild / sbtBinaryVersion).value match {
          case "0.13" => Versions.scala210
          case _      => Versions.scala212
        }
      },
      bintrayOrganization := Some("scala-native-bindgen"),
      bintrayRepository := {
        if (Keys.sbtPlugin.value) "sbt-plugins"
        else "maven"
      },
      publishMavenStyle := Keys.sbtPlugin.value == false,
      Test / publishArtifact := false
    )
}

lazy val setReleaseVersions: String => State => State =
  v => _.put(ReleaseKeys.versions, (v, v))
