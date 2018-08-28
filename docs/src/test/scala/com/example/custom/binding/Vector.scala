//#example
package com.example.custom.binding

import scala.scalanative._
import scala.scalanative.native._

@native.link("vector")
@native.extern
object Vector {
  type Point       = native.CStruct2[native.CFloat, native.CFloat]
  type LineSegment = native.CStruct2[Point, Point]
  // ...
  //#example
  def cosine(v1: native.Ptr[LineSegment],
             v2: native.Ptr[LineSegment]): native.CFloat = native.extern

  object implicits {
    implicit class PointOps(val p: native.Ptr[Point]) extends AnyVal {
      def x: native.CFloat                = !p._1
      def x_=(value: native.CFloat): Unit = !p._1 = value
      def y: native.CFloat                = !p._2
      def y_=(value: native.CFloat): Unit = !p._2 = value
    }

    implicit class LineSegmentOps(val p: native.Ptr[LineSegment])
        extends AnyVal {
      def a: native.Ptr[Point]                = p._1
      def a_=(value: native.Ptr[Point]): Unit = !p._1 = !value
      def b: native.Ptr[Point]                = p._2
      def b_=(value: native.Ptr[Point]): Unit = !p._2 = !value
    }
  }

  object Point {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[Point] =
      native.alloc[Point]
    def apply(x: native.CFloat, y: native.CFloat)(
        implicit z: native.Zone): native.Ptr[Point] = {
      val ptr = native.alloc[Point]
      ptr.x = x
      ptr.y = y
      ptr
    }
  }

  object LineSegment {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[LineSegment] =
      native.alloc[LineSegment]
    def apply(a: native.Ptr[Point], b: native.Ptr[Point])(
        implicit z: native.Zone): native.Ptr[LineSegment] = {
      val ptr = native.alloc[LineSegment]
      ptr.a = a
      ptr.b = b
      ptr
    }
  }
  //#example
}
//#example
