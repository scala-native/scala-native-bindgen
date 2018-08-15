package org.example

import scala.scalanative._
import scala.scalanative.native._

@native.link("vector")
@native.extern
object vector {
  type struct_point = native.CStruct2[native.CFloat, native.CFloat]
  type struct_lineSegment = native.CStruct2[struct_point, struct_point]
  def cosine(v1: native.Ptr[struct_lineSegment], v2: native.Ptr[struct_lineSegment]): native.CFloat = native.extern

  object implicits {
    implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
      def x: native.CFloat = !p._1
      def x_=(value: native.CFloat): Unit = !p._1 = value
      def y: native.CFloat = !p._2
      def y_=(value: native.CFloat): Unit = !p._2 = value
    }

    implicit class struct_lineSegment_ops(val p: native.Ptr[struct_lineSegment]) extends AnyVal {
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

  object struct_lineSegment {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_lineSegment] = native.alloc[struct_lineSegment]
    def apply(a: native.Ptr[struct_point], b: native.Ptr[struct_point])(implicit z: native.Zone): native.Ptr[struct_lineSegment] = {
      val ptr = native.alloc[struct_lineSegment]
      ptr.a = a
      ptr.b = b
      ptr
    }
  }
}
