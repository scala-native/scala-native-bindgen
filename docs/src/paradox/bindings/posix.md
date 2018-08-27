# POSIX

An addition to Scala Native's [POSIX](http://www.scala-native.org/en/latest/lib/posixlib.html) bindings. To use one of the POSIX bindings you must add the resolver and the dependency:

sbt
:   @@snip [build.sbt](../resources/build.sbt) { #library_resolver }

Maven
:   @@snip [pom.xml](../resources/pom.xml) { #library_resolver }

Gradle
:   @@snip [build.gradle](../resources/build.gradle) { #library_resolver }


@@dependency[sbt,Maven,Gradle] {
  group="org.scala-native.bindgen"
  artifact="libposix_$scala.binary.version$"
  version="$project.version$"
}

Binding objects are available under the package name `

| Header               | Description
|----------------------|------------------------------------------------------|
| [`fnmatch.h`]        | Check if a file or path name matches a shell pattern
| [`regex.h`]          | Regular expression library

 [`fnmatch.h`]: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/fnmatch.h.html
 [`regex.h`]: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/regex.h.html

## Examples

Using `fnmatch.h`:

@@snip [fnmatch](../../../../bindings/posix/src/test/scala/org/scalanative/bindgen/bindings/tests/FnmatchSpec.scala) { #usage-example }

Using `regex.h`:

@@snip [regex](../../../../bindings/posix/src/test/scala/org/scalanative/bindgen/bindings/tests/RegexSpec.scala) { #usage-example }
