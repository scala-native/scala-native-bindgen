package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object OpaqueTypes {
  type struct_point = native.CStruct2[native.CInt, native.CInt]
  type struct_points = native.CStruct2[native.Ptr[struct_point], native.Ptr[struct_point]]
  type points = struct_points
  type union_u = native.CArray[Byte, native.Nat._4]
  type u = union_u
  def move(point: native.Ptr[struct_point], x: native.CInt, y: native.CInt): native.Ptr[struct_point] = native.extern
  def processPoints(p: native.Ptr[points]): native.Ptr[union_u] = native.extern
}

import OpaqueTypes._

object OpaqueTypesHelpers {

  implicit class struct_points_ops(val p: native.Ptr[struct_points]) extends AnyVal {
    def point1: native.Ptr[struct_point] = !p._1
    def point1_=(value: native.Ptr[struct_point]): Unit = !p._1 = value
    def point2: native.Ptr[struct_point] = !p._2
    def point2_=(value: native.Ptr[struct_point]): Unit = !p._2 = value
  }

  def struct_points()(implicit z: native.Zone): native.Ptr[struct_points] = native.alloc[struct_points]

  implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
    def x: native.CInt = !p._1
    def x_=(value: native.CInt): Unit = !p._1 = value
    def y: native.CInt = !p._2
    def y_=(value: native.CInt): Unit = !p._2 = value
  }

  def struct_point()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]

  implicit class union_u_pos(val p: native.Ptr[union_u]) extends AnyVal {
    def i: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
    def i_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    def f: native.Ptr[native.CFloat] = p.cast[native.Ptr[native.CFloat]]
    def f_=(value: native.CFloat): Unit = !p.cast[native.Ptr[native.CFloat]] = value
  }
}
