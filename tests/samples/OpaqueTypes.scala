package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object OpaqueTypes {
  type struct_undefinedIncludedStruct = native.CStruct0 // incomplete type
  type undefinedIncludedStruct = struct_undefinedIncludedStruct
  type struct_points = native.CStruct2[native.Ptr[struct_point], native.Ptr[struct_point]]
  type points = struct_points
  type struct_point = native.CStruct2[native.CInt, native.CInt]
  type union_u = native.CArray[Byte, native.Nat._4]
  type u = union_u
  type struct_undefinedStruct = native.CStruct0 // incomplete type
  type struct_structWithPointerToUndefinedStruct = native.CStruct1[native.Ptr[struct_undefinedStruct]]
  type union_unionWithPointerToUndefinedStruct = native.CArray[Byte, native.Nat._8]
  type union_undefinedUnion = native.CStruct0 // incomplete type
  type undefinedUnion = union_undefinedUnion
  type aliasToPointerOfUndefinedUnion = native.Ptr[undefinedUnion]
  type aliasForUndefinedStruct = struct_undefinedStruct
  type functionPointerWithPointerToOpaqueType = native.CFunctionPtr1[native.Ptr[native.Ptr[struct_undefinedStruct]], native.Ptr[undefinedUnion]]
  def move(point: native.Ptr[struct_point], x: native.CInt, y: native.CInt): native.Ptr[struct_point] = native.extern
  def processPoints(p: native.Ptr[points]): native.Ptr[union_u] = native.extern
  def usePointerToUndefinedStruct(p0: native.Ptr[struct_undefinedStruct]): Unit = native.extern
  def fun(): native.Ptr[native.Ptr[undefinedUnion]] = native.extern
  def returnPointerToAliasOfUndefinedStruct(): native.Ptr[aliasForUndefinedStruct] = native.extern
  def usePointerToUndefinedIncludedStruct(p0: native.Ptr[undefinedIncludedStruct]): Unit = native.extern
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

  implicit class struct_structWithPointerToUndefinedStruct_ops(val p: native.Ptr[struct_structWithPointerToUndefinedStruct]) extends AnyVal {
    def field: native.Ptr[struct_undefinedStruct] = !p._1
    def field_=(value: native.Ptr[struct_undefinedStruct]): Unit = !p._1 = value
  }

  def struct_structWithPointerToUndefinedStruct()(implicit z: native.Zone): native.Ptr[struct_structWithPointerToUndefinedStruct] = native.alloc[struct_structWithPointerToUndefinedStruct]

  implicit class union_u_pos(val p: native.Ptr[union_u]) extends AnyVal {
    def i: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
    def i_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    def f: native.Ptr[native.CFloat] = p.cast[native.Ptr[native.CFloat]]
    def f_=(value: native.CFloat): Unit = !p.cast[native.Ptr[native.CFloat]] = value
  }

  implicit class union_unionWithPointerToUndefinedStruct_pos(val p: native.Ptr[union_unionWithPointerToUndefinedStruct]) extends AnyVal {
    def field: native.Ptr[native.Ptr[struct_undefinedStruct]] = p.cast[native.Ptr[native.Ptr[struct_undefinedStruct]]]
    def field_=(value: native.Ptr[struct_undefinedStruct]): Unit = !p.cast[native.Ptr[native.Ptr[struct_undefinedStruct]]] = value
  }
}
