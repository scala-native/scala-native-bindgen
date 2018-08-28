# Scala Native Binding Generator

[![Build Status](https://travis-ci.com/kornilova-l/scala-native-bindgen.svg?branch=master)](https://travis-ci.com/kornilova-l/scala-native-bindgen)

The tool generates Scala Native bindings from C headers.

## Documentation

Documentation can be found at [scala-native.github.io/scala-native-bindgen](https://scala-native.github.io/scala-native-bindgen/).

## Bindgen Features

* possibility to reuse types from existing bindings.
* type casts that make recursive structs be valid Scala Native structs.
* implicit classes for structs and unions that make fields access easier.
* implicit classes that add setters and getters to structs with more than 22 fields (such structs in Scala
  Native are represented as arrays of bytes).
* literal defines embedding `#define MY_CONSTANT 42` → `val MY_CONSTANT: native.CInt = 42`.
* read-only bindings for extern variables (such variables cannot be updated due to Scala Native limitation).
* declarations filtering by prefix.

## Example

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

```scala
import scala.scalanative._
import scala.scalanative.native._

@native.link("vector")
@native.extern
object vector {
  type struct_point = native.CStruct2[native.CFloat, native.CFloat]
  type struct_vector = native.CStruct2[struct_point, struct_point]
  def add(v1: native.Ptr[struct_vector], v2: native.Ptr[struct_vector]): native.Ptr[struct_vector] = native.extern

  object implicits {
    implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
      def x: native.CFloat = !p._1
      def x_=(value: native.CFloat): Unit = !p._1 = value
      def y: native.CFloat = !p._2
      def y_=(value: native.CFloat): Unit = !p._2 = value
    }

    implicit class struct_vector_ops(val p: native.Ptr[struct_vector]) extends AnyVal {
      def a: native.Ptr[struct_point] = p._1
      def a_=(value: native.Ptr[struct_point]): Unit = !p._1 = !value
      def b: native.Ptr[struct_point] = p._2
      def b_=(value: native.Ptr[struct_point]): Unit = !p._2 = !value
    }
  }

  object struct_point {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]
    def apply(x: native.CFloat, y: native.CFloat)(implicit z: native.Zone): native.Ptr[struct_point] = {
      val ptr = native.alloc[struct_point]
      ptr.x = x
      ptr.y = y
      ptr
    }
  }

  object struct_vector {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_vector] = native.alloc[struct_vector]
    def apply(a: native.Ptr[struct_point], b: native.Ptr[struct_point])(implicit z: native.Zone): native.Ptr[struct_vector] = {
      val ptr = native.alloc[struct_vector]
      ptr.a = a
      ptr.b = b
      ptr
    }
  }
}
```

## License

This project is distributed under the Scala license.
[See LICENSE.txt for details](LICENSE.txt)
