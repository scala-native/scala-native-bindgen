# utf8proc

The [utf8proc] binding provides Unicode normalization, case-folding, and other operations for strings in the UTF-8 encoding.

To use this binding add the following resolver and dependency:

@@binding[utf8proc]

## Example

@@snip [iconv](../../../../bindings/utf8proc/src/test/scala/org/scalanative/bindgen/bindings/tests/Utf8procSpec.scala) { #usage-example }

 [utf8proc]: https://juliastrings.github.io/utf8proc/doc/