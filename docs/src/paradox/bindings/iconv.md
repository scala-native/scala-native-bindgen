# iconv

The [`iconv.h`] binding allows to convert text between different character sets, for example convert text from UTF-8 to ISO-8859-1.

To use this binding add the following resolver and the dependency:

@@binding[iconv]

## Example

@@snip [iconv](../../../../bindings/iconv/src/test/scala/org/scalanative/bindgen/bindings/tests/IconvSpec.scala) { #usage-example }

 [`iconv.h`]: http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/iconv.h.html
