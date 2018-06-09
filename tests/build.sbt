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

val `scala-native-bindgen-tests` = project
  .in(file("."))
  .aggregate(samples)
  .settings(
    fork in Test := true,
    javaOptions in Test += "-Dbindgen.path=" + file(
      "../bindgen/target/scala-native-bindgen"),
    watchSources += WatchSource(
      baseDirectory.value / "samples",
      "*.h" || "*.scala",
      NothingFilter
    ),
    libraryDependencies += "org.scalatest" %% "scalatest" % "3.0.5" % Test
  )

lazy val samples = project
  .enablePlugins(ScalaNativePlugin)
  .settings(test := (compile in Compile).value)
