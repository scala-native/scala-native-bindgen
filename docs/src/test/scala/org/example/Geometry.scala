package org.example.geometry

import scala.scalanative._
import scala.scalanative.native._

@native.link("geometry")
@native.extern
object Geometry {
  //#using-struct-point
  type struct_circle = native.CStruct2[com.example.custom.binding.Vector.Point, native.CDouble]
  //#using-struct-point
  def circle_area(circle: native.Ptr[struct_circle]): native.CFloat = native.extern

  object implicits {
    implicit class struct_circle_ops(val p: native.Ptr[struct_circle]) extends AnyVal {
      def point: native.Ptr[com.example.custom.binding.Vector.Point] = p._1
      def point_=(value: native.Ptr[com.example.custom.binding.Vector.Point]): Unit = !p._1 = !value
      def radius: native.CDouble = !p._2
      def radius_=(value: native.CDouble): Unit = !p._2 = value
    }
  }

  object struct_circle {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_circle] = native.alloc[struct_circle]
    def apply(point: native.Ptr[com.example.custom.binding.Vector.Point], radius: native.CDouble)(implicit z: native.Zone): native.Ptr[struct_circle] = {
      val ptr = native.alloc[struct_circle]
      ptr.point = point
      ptr.radius = radius
      ptr
    }
  }
}
