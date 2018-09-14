# POSIX

An addition to Scala Native's [POSIX](http://www.scala-native.org/en/latest/lib/posixlib.html) bindings.

To use this binding add the following resolver and dependency:

@@binding[posix]

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
