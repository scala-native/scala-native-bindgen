# Iconv - Character set conversion

To use this binding add the following dependency:

@@dependency[sbt,Maven,Gradle] {
  group="org.scala-native.bindgen"
  artifact="libposix_$scala.binary.version$"
  version="$project.version$"
}

The [`iconv.h`] header allows to convert text between different character sets, for example UTF-8 to ISO-8859-1.

## Example

@@snip [iconv](../../../../bindings/iconv/src/test/scala/org/scalanative/bindgen/bindings/tests/IconvSpec.scala) { #usage-example }

 [`iconv.h`]: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/iconv.h.html
