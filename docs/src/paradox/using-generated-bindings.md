# Using Generated Bindings

Consider following header file:

@@snip [vector.h] (../test/resources/vector.h)

Bindgen will generate type aliases for the structs, binding for function `add`
and helper functions that make usage of structs easier.

@@snip [vector.h] (../test/scala/org/example/vector.scala)

Let's write code that creates two vectors, adds them and prints resulting
vector.

First we need to create points for vectors. We will use `native.Zone` to
allocate struct (more information on memory management can be found
here: [Scala Native memory management]).

Helper object `mylibHelpers` contains function for struct allocation.
To import it use `import mylibHelpers._`

Let's create points for first vector:

@@snip [step-1] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-1 }

Now we want to set fields of created points. Scala Native provides access
to fields by using `_N` methods where `N` is index of a field
(see [Scala Native memory layout types]).

Bindgen generates implicit helper classes that wrap calls to `_N` in functions
with meaningful names. We already imported helper class, so we can use the
functions:

@@snip [step-2] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-2 }

Lets create first vector. Note that `struct_vector` contains
fields of type `struct_point` but setters accept variables of type
`native.Ptr[struct_point]`. It helps to avoid Scala Native limitation that
does not allow passing structs and arrays by value
(see @github[scala-native/scala-native#555](scala-native/scala-native#555)).

@@snip [step-3] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-3 }

Repeat these steps to create second vector. Once both vectors are created we can
call `add` function and print the result:

@@snip [step-4] (../test/scala/org/scalanative/bindgen/docs/VectorSpec.scala) { #step-4 }

 [Scala Native memory management]: http://www.scala-native.org/en/latest/user/interop.html#memory-management
 [Scala Native memory layout types]: http://www.scala-native.org/en/latest/user/interop.html#memory-layout-types
