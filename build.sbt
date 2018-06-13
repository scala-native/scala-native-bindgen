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
  .settings(test := (compile in Compile).value)
