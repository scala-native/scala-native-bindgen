# Configuration

Bindgen provides several options for configuring what to include and how
to output the generated bindings. The options can be provided to the CLI
or as part of the sbt bindings declaration.

@@ toc

## Excluding Definitions By Prefix

Definitions may be excluded by their prefix. This is useful when private definitions should not be part of the generated binding. This is often the case for definitions starting with `__`.

sbt
:   @@snip [sbt] (../../../sbt-scala-native-bindgen/src/sbt-test/bindgen/generate/build.sbt) { #sbt-exclude-prefix }

CLI
:   ```sh
    scala-native-bindgen --binding-config=/path/to/config
    ```

To exemplify consider the following header file:

@@snip [sbt] (../../../sbt-scala-native-bindgen/src/sbt-test/bindgen/generate/src/main/resources/stdlib.h)

When the exclude prefix is set to `__`, then the resulting bindings will be:

@@snip [sbt] (../../../sbt-scala-native-bindgen/src/sbt-test/bindgen/generate/expected/stdlib.scala)

## Binding Configuration File

The binding configuration is a YAML file which allows to map the path of
a header file to the associated object as well as the names of the C
types and symbols to their respective Scala types and definitions. The
configuration file can be used when integrating with third party
bindings.

sbt
:   @@snip [sbt-binding-config](../../../build.sbt) { #sbt-binding-config }

CLI
:   ```sh
    scala-native-bindgen --binding-config=/path/to/config
    ```

### Using Types From Third Party Bindings

If you need to generate bindings that uses types from bindings that have not been generated with `scala-native-bindgen` you have to provide the mapping between the header path and the Scala object's fully qualified name. And in some cases also the Scala name of the C types. Using the vector library example from @ref:[Using Bindings](using-bindings.md), let's assume that the vector library was created so that `struct point` was named `Point`:

@@snip [Vector] (../test/scala/com/example/custom/binding/Vector.scala) { #example }

To use this binding, create a configuration file with the folllowing content, where `path` is the name of the header file (usually the part of the path inside the `/usr/include` or `/usr/local/include` directory), `object` is the fully qualified name of the Scala object (i.e. package name as well as the Scala object name) and finally `names` for each of the types:

@@snip [vector.h] (../test/resources/3rd-party-bindings/config.yml)

Now in the library you are creating a binding for, any usage of `struct point`:

@@snip [vector.h] (../test/resources/3rd-party-bindings/geometry.h) { #using-struct-point }

will reference the `Point` type alias inside the `Vector` object:

@@snip [Geometry] (../test/scala/org/example/Geometry.scala) { #using-struct-point }

### Using Types From the Scala Native Bindings

Similar to the above, the following this example shows how you can use
types defined in the [C standard library] and [C POSIX library] bindings
shipped with Scala Native. Let's assume we have a binding with a method that uses the `FILE` type
from `<stdio.h>`:

@@snip [vector.h] (../test/resources/scala-native-bindings/wordcount.h) { #using-stdio-file }

We can then write a binding configuration that maps the header name to the object defined in Scala Native.

@@snip [vector.h] (../test/resources/scala-native-bindings/config.yml)

@@@ note

In the above binding configuration, we duplicate the mapping to work on both Linux and macOS since on macOS
the definition of `FILE` is found inside `/usr/include/_stdio.h`.

@@@

The generated binding will then use the `stdio.h` binding provided by Scala Native:

@@snip [WordCount] (../test/scala/org/example/WordCount.scala) { #using-stdio-file }

And we can use the binding by opening a file using `fopen(...)`:

@@snip [WordCountSpec] (../test/scala/org/scalanative/bindgen/docs/WordCountSpec.scala) { #example }

 [Scala Native memory management]: http://www.scala-native.org/en/latest/user/interop.html#memory-management
 [Scala Native memory layout types]: http://www.scala-native.org/en/latest/user/interop.html#memory-layout-types
 [C standard library]: http://www.scala-native.org/en/latest/lib/libc.html
 [C POSIX library]: http://www.scala-native.org/en/latest/lib/posixlib.html
