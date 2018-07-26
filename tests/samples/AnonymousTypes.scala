package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object AnonymousTypes {
  type struct_anonymous_0 = native.CStruct1[native.CChar]
  type union_anonymous_0 = native.CArray[Byte, native.Nat._8]
  type struct_anonymous_1 = native.CStruct1[native.Ptr[union_anonymous_0]]
  type struct_StructWithAnonymousStruct = native.CStruct2[native.Ptr[struct_anonymous_1], native.CUnsignedInt]
  type struct_anonymous_2 = native.CStruct1[native.CInt]
  def foo(s: native.Ptr[struct_anonymous_0]): Unit = native.extern
  def bar(): native.Ptr[struct_anonymous_2] = native.extern
}

import AnonymousTypes._

object AnonymousTypesHelpers {

  implicit class struct_anonymous_0_ops(val p: native.Ptr[struct_anonymous_0]) extends AnyVal {
    def a: native.CChar = !p._1
    def a_=(value: native.CChar): Unit = !p._1 = value
  }

  def struct_anonymous_0()(implicit z: native.Zone): native.Ptr[struct_anonymous_0] = native.alloc[struct_anonymous_0]

  implicit class struct_anonymous_1_ops(val p: native.Ptr[struct_anonymous_1]) extends AnyVal {
    def innerUnion: native.Ptr[union_anonymous_0] = !p._1
    def innerUnion_=(value: native.Ptr[union_anonymous_0]): Unit = !p._1 = value
  }

  def struct_anonymous_1()(implicit z: native.Zone): native.Ptr[struct_anonymous_1] = native.alloc[struct_anonymous_1]

  implicit class struct_StructWithAnonymousStruct_ops(val p: native.Ptr[struct_StructWithAnonymousStruct]) extends AnyVal {
    def innerStruct: native.Ptr[struct_anonymous_1] = !p._1
    def innerStruct_=(value: native.Ptr[struct_anonymous_1]): Unit = !p._1 = value
    def innerEnum: native.CUnsignedInt = !p._2
    def innerEnum_=(value: native.CUnsignedInt): Unit = !p._2 = value
  }

  def struct_StructWithAnonymousStruct()(implicit z: native.Zone): native.Ptr[struct_StructWithAnonymousStruct] = native.alloc[struct_StructWithAnonymousStruct]

  implicit class struct_anonymous_2_ops(val p: native.Ptr[struct_anonymous_2]) extends AnyVal {
    def result: native.CInt = !p._1
    def result_=(value: native.CInt): Unit = !p._1 = value
  }

  def struct_anonymous_2()(implicit z: native.Zone): native.Ptr[struct_anonymous_2] = native.alloc[struct_anonymous_2]

  implicit class union_anonymous_0_pos(val p: native.Ptr[union_anonymous_0]) extends AnyVal {
    def a: native.Ptr[native.CLong] = p.cast[native.Ptr[native.CLong]]
    def a_=(value: native.CLong): Unit = !p.cast[native.Ptr[native.CLong]] = value
  }
}
