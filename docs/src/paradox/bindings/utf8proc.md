# Utf8proc - UTF-8 text processing

This binding for [`utf8proc.h`] provides Unicode normalization, case-folding, and other operations for strings in the UTF-8 encoding.

To use it add the following dependency:

@@dependency[sbt,Maven,Gradle] {
  group="org.scala-native.bindgen"
  artifact="libutf8proc_$scala.binary.version$"
  version="$project.version$"
}

## Example

@@snip [iconv](../../../../bindings/utf8proc/src/test/scala/org/scalanative/bindgen/bindings/tests/Utf8procSpec.scala) { #usage-example }

 [`utf8proc.h`]: https://juliastrings.github.io/utf8proc/doc/