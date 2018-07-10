addSbtPlugin("org.scala-native" % "sbt-scala-native"           % "0.3.7")
addSbtPlugin("com.typesafe.sbt" % "sbt-site"                   % "1.3.2")
addSbtPlugin("io.github.jonas"  % "sbt-paradox-material-theme" % "0.4.0")
addSbtPlugin("com.typesafe.sbt" % "sbt-ghpages"                % "0.6.2")

libraryDependencies += "org.scala-sbt" %% "scripted-plugin" % sbtVersion.value
