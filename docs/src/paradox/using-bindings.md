# Using Bindings

The following will explain

## A Simple Vector Library

Consider following header file:

@@snip [vector.h] (../test/resources/using-bindings/vector.h)

Bindgen will output

 * type aliases for the structs
 * binding for function `cosine`
 * helper functions that make usage of structs easier

@@snip [vector.h] (../test/scala/org/example/vector.scala)

## Using the Vector Library

Let's write code that creates two line segments and calculates the angel between them.

First we need to create points. We will use `native.Zone` to
allocate struct (more information on memory management can be found
here: [Scala Native memory management]).

The generated bindings contain helper functions that make struct allocation easier.
To import them use `import org.example.vector._`

Let's create two points and the first line segment:

@@snip [step-1] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-1 }

There is no need to create points manually, just call `struct_lineSegment`
constructor and set point coordinates using fields setters.

Scala Native allows us to access a field by using `_N` method where `N` is index of a field
(see [Scala Native memory layout types]) but it is not convenient because we have to
match indices with fields names.

Bindgen provides implicit helper classes that wrap calls to `_N` in functions
with meaningful names. To import these classes add `import org.example.vector.implicits._`
to your code:

@@snip [step-2] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-2 }

@@@ note

`struct_lineSegment` contains fields of value type `struct_point`
but setters accept variables of type `native.Ptr[struct_point]`.
The reason for this is that Scala Native does not allow passing structs
and arrays by value (see @github[scala-native/scala-native#555](scala-native/scala-native#555)).

@@@

Now we can calculate the angel between the line segments:

@@snip [step-3] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-3 }
