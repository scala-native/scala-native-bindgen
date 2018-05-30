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
  .settings(
    fork in Test := true,
    javaOptions in Test += "-Dbindgen.path=" + file("../target/scalaBindgen"),
    watchSources += WatchSource(
      baseDirectory.value / "samples",
      "*.h" || "*.scala",
      NothingFilter
    ),
    libraryDependencies += "org.scalatest" %% "scalatest" % "3.0.5" % Test
  )

val samples = project
  .enablePlugins(ScalaNativePlugin)
