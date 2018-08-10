package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Union {
  type enum_union_op = native.CUnsignedInt
  object enum_union_op {
    final val UNION_SET: enum_union_op = 0.toUInt
    final val UNION_TEST: enum_union_op = 1.toUInt
  }

  type struct_s = native.CStruct1[native.CInt]
  type union_values = native.CArray[Byte, native.Nat._8]
  def union_get_sizeof(): native.CInt = native.extern
  def union_test_int(v: native.Ptr[union_values], op: enum_union_op, value: native.CInt): native.CInt = native.extern
  def union_test_long(v: native.Ptr[union_values], op: enum_union_op, value: native.CLong): native.CInt = native.extern
  def union_test_long_long(v: native.Ptr[union_values], op: enum_union_op, value: native.CLongLong): native.CInt = native.extern
  def union_test_double(v: native.Ptr[union_values], op: enum_union_op, value: native.CDouble): native.CInt = native.extern
  def union_test_string(v: native.Ptr[union_values], op: enum_union_op, value: native.CString): native.CInt = native.extern
  def union_test_struct(v: native.Ptr[union_values], op: enum_union_op, value: native.Ptr[struct_s]): native.CInt = native.extern
}

import Union._

object UnionHelpers {

  implicit class struct_s_ops(val p: native.Ptr[struct_s]) extends AnyVal {
    def a: native.CInt = !p._1
    def a_=(value: native.CInt): Unit = !p._1 = value
  }

  def struct_s()(implicit z: native.Zone): native.Ptr[struct_s] = native.alloc[struct_s]

  implicit class union_values_pos(val p: native.Ptr[union_values]) extends AnyVal {
    def l: native.Ptr[native.CLong] = p.cast[native.Ptr[native.CLong]]
    def l_=(value: native.CLong): Unit = !p.cast[native.Ptr[native.CLong]] = value
    def i: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
    def i_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    def ll: native.Ptr[native.CLongLong] = p.cast[native.Ptr[native.CLongLong]]
    def ll_=(value: native.CLongLong): Unit = !p.cast[native.Ptr[native.CLongLong]] = value
    def d: native.Ptr[native.CDouble] = p.cast[native.Ptr[native.CDouble]]
    def d_=(value: native.CDouble): Unit = !p.cast[native.Ptr[native.CDouble]] = value
    def s: native.Ptr[native.CString] = p.cast[native.Ptr[native.CString]]
    def s_=(value: native.CString): Unit = !p.cast[native.Ptr[native.CString]] = value
    def structInUnion: native.Ptr[struct_s] = p.cast[native.Ptr[struct_s]]
    def structInUnion_=(value: native.Ptr[struct_s]): Unit = !p.cast[native.Ptr[struct_s]] = !value
  }
}
