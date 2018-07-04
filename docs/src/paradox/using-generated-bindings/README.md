# Using Generated Bindings

Consider following header file:

```c
struct point {
    float x;
    float y;
};

struct vector {
    struct point a;
    struct point b;
};

struct vector *add(struct vector *v1, struct vector *v2);
```

Bindgen will generate type aliases for the structs, binding for function `add`
and helper functions that make usage of structs easier.
```scala
import scala.scalanative._
import scala.scalanative.native._

@native.link("mylib")
@native.extern
object mylib {
  type struct_point = native.CStruct2[native.CFloat, native.CFloat]
  type struct_vector = native.CStruct2[struct_point, struct_point]
  def add(v1: native.Ptr[struct_vector], v2: native.Ptr[struct_vector]): native.Ptr[struct_vector] = native.extern
}

import mylib._

object mylibHelpers {

  implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
    def x: native.CFloat = !p._1
    def x_=(value: native.CFloat): Unit = !p._1 = value
    def y: native.CFloat = !p._2
    def y_=(value: native.CFloat): Unit = !p._2 = value
  }

  def struct_point()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]

  implicit class struct_vector_ops(val p: native.Ptr[struct_vector]) extends AnyVal {
    def a: native.Ptr[struct_point] = p._1
    def a_=(value: native.Ptr[struct_point]): Unit = !p._1 = !value
    def b: native.Ptr[struct_point] = p._2
    def b_=(value: native.Ptr[struct_point]): Unit = !p._2 = !value
  }

  def struct_vector()(implicit z: native.Zone): native.Ptr[struct_vector] = native.alloc[struct_vector]
}
```
Let's write code that creates two vectors, adds them and prints resulting
vector.

First we need to create points for vectors. We will use `native.Zone` to
allocate struct (more information on memory management can be found
here: [Scala Native memory management]).

Helper object `mylibHelpers` contains function for struct allocation.
To import it use `import mylibHelpers._`

Let's create points for first vector:
```scala
import mylibHelpers._
import scala.scalanative.native.Zone

object Hello extends App {
  Zone { implicit zone =>
   val vec1p1 = struct_point()
   val vec1p2 = struct_point()
  }
}
```

Now we want to set fields of created points. Scala Native provides access
to fields by using `_N` methods where `N` is index of a field
(see [Scala Native memory layout types]).

Bindgen generates implicit helper classes that wrap calls to `_N` in functions
with meaningful names. We already imported helper class, so we can use the
functions:
```scala
vec1p1.x = 0
vec1p1.y = 1

vec1p2.x = 6
vec1p2.y = 3
```

Lets create first vector. Note that `struct_vector` contains
fields of type `struct_point` but setters accept variables of type
`native.Ptr[struct_point]`. It helps to avoid Scala Native limitation that
does not allow passing structs and arrays by value
(see @github[scala-native/scala-native#555](scala-native/scala-native#555)).
```scala
val vec1 = struct_vector()
vec1.a = vec1p1
vec1.b = vec1p2
```
Repeat these steps to create second vector. Once both vectors are created we can
call `add` function and print the result:
```scala
val vec3 = mylib.add(vec1, vec2)
println(s"(${vec3.a.x}, ${vec3.a.y}), (${vec3.b.x}, ${vec3.b.y})")
```

 [Scala Native memory management]: http://www.scala-native.org/en/latest/user/interop.html#memory-management
 [Scala Native memory layout types]: http://www.scala-native.org/en/latest/user/interop.html#memory-layout-types
