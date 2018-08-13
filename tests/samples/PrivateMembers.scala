package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object PrivateMembers {
  type enum___privateEnum = native.CUnsignedInt
  object enum___privateEnum {
    final val A: enum___privateEnum = 0.toUInt
    final val B: enum___privateEnum = 1.toUInt
  }

  type enum_enumWithPrivateMembers = native.CUnsignedInt
  object enum_enumWithPrivateMembers {
    final val __C: enum_enumWithPrivateMembers = 0.toUInt
    final val D: enum_enumWithPrivateMembers = 1.toUInt
  }

  type enum_anonymous_0 = native.CUnsignedInt
  object enum_anonymous_0 {
    final val __E: enum_anonymous_0 = 0.toUInt
    final val F: enum_anonymous_0 = 1.toUInt
  }

  type pid_t = native.CInt
  type __private_type = native.CInt
  type struct_structWithPrivateType = native.CStruct2[native.CInt, __private_type]
  type union___unionWithPrivateName = native.CArray[Byte, native.Nat._4]
  type struct_structWithPrivateStruct = native.CStruct1[native.Ptr[struct_structWithPrivateType]]
  type struct_normalStruct = native.CStruct1[native.CInt]
  type struct_privateStructWithTypedef = native.CStruct1[native.Ptr[__private_type]]
  type privateStructWithTypedef = struct_privateStructWithTypedef
  type privateStructWithTypedefPtr = native.Ptr[struct_privateStructWithTypedef]
  def getTypeThatUsesPrivateTypes(): pid_t = native.extern
  def getPrivateType(): native.Ptr[__private_type] = native.extern
  def usesPrivateUnion(p0: native.Ptr[union___unionWithPrivateName]): Unit = native.extern
  def usesPrivateStruct(p0: native.Ptr[struct_structWithPrivateType], p1: native.Ptr[struct_normalStruct]): Unit = native.extern
  def usesPrivateEnum(p0: native.Ptr[enum___privateEnum]): Unit = native.extern

  object implicits {
    implicit class struct_structWithPrivateType_ops(val p: native.Ptr[struct_structWithPrivateType]) extends AnyVal {
      def field1: native.CInt = !p._1
      def field1_=(value: native.CInt): Unit = !p._1 = value
      def field2: __private_type = !p._2
      def field2_=(value: __private_type): Unit = !p._2 = value
    }

    implicit class struct_structWithPrivateStruct_ops(val p: native.Ptr[struct_structWithPrivateStruct]) extends AnyVal {
      def s: native.Ptr[struct_structWithPrivateType] = !p._1
      def s_=(value: native.Ptr[struct_structWithPrivateType]): Unit = !p._1 = value
    }

    implicit class struct_normalStruct_ops(val p: native.Ptr[struct_normalStruct]) extends AnyVal {
      def a: native.CInt = !p._1
      def a_=(value: native.CInt): Unit = !p._1 = value
    }

    implicit class struct_privateStructWithTypedef_ops(val p: native.Ptr[struct_privateStructWithTypedef]) extends AnyVal {
      def a: native.Ptr[__private_type] = !p._1
      def a_=(value: native.Ptr[__private_type]): Unit = !p._1 = value
    }

    implicit class union___unionWithPrivateName_pos(val p: native.Ptr[union___unionWithPrivateName]) extends AnyVal {
      def a: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
      def a_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    }
  }

  object struct_structWithPrivateType {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_structWithPrivateType] = native.alloc[struct_structWithPrivateType]
    def apply(field1: native.CInt, field2: __private_type)(implicit z: native.Zone): native.Ptr[struct_structWithPrivateType] = {
      val ptr = native.alloc[struct_structWithPrivateType]
      ptr.field1 = field1
      ptr.field2 = field2
      ptr
    }
  }

  object struct_structWithPrivateStruct {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_structWithPrivateStruct] = native.alloc[struct_structWithPrivateStruct]
    def apply(s: native.Ptr[struct_structWithPrivateType])(implicit z: native.Zone): native.Ptr[struct_structWithPrivateStruct] = {
      val ptr = native.alloc[struct_structWithPrivateStruct]
      ptr.s = s
      ptr
    }
  }

  object struct_normalStruct {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_normalStruct] = native.alloc[struct_normalStruct]
    def apply(a: native.CInt)(implicit z: native.Zone): native.Ptr[struct_normalStruct] = {
      val ptr = native.alloc[struct_normalStruct]
      ptr.a = a
      ptr
    }
  }

  object struct_privateStructWithTypedef {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_privateStructWithTypedef] = native.alloc[struct_privateStructWithTypedef]
    def apply(a: native.Ptr[__private_type])(implicit z: native.Zone): native.Ptr[struct_privateStructWithTypedef] = {
      val ptr = native.alloc[struct_privateStructWithTypedef]
      ptr.a = a
      ptr
    }
  }
}
