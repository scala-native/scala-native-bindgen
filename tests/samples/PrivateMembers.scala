import scala.scalanative._
import scala.scalanative.native._
import scala.scalanative.native.Nat._

@native.link("PrivateMembers")
@native.extern
object PrivateMembers {
  type pid_t = native.CInt
  type __private_type = native.CInt
  type privateStructWithTypedef = struct_privateStructWithTypedef
  type privateStructWithTypedefPtr = native.Ptr[struct_privateStructWithTypedef]
  type enum___privateEnum = native.CUnsignedInt
  type enum_enumWithPrivateMembers = native.CUnsignedInt
  type struct_structWithPrivateType = native.CStruct2[native.CInt, __private_type]
  type struct_structWithPrivateStruct = native.CStruct1[native.Ptr[struct_structWithPrivateType]]
  type struct_normalStruct = native.CStruct1[native.CInt]
  type struct_privateStructWithTypedef = native.CStruct1[native.Ptr[__private_type]]
  type union___unionWithPrivateName = native.CArray[Byte, Digit[_3, _2]]
  def getTypeThatUsesPrivateTypes(): pid_t = native.extern
  def getPrivateType(): native.Ptr[__private_type] = native.extern
  def usesPrivateUnion(anonymous0: union___unionWithPrivateName): Unit = native.extern
  def usesPrivateStruct(anonymous0: native.Ptr[struct_structWithPrivateType], anonymous1: native.Ptr[struct_normalStruct]): Unit = native.extern
  def usesPrivateEnum(anonymous0: native.Ptr[enum___privateEnum]): Unit = native.extern
}

import PrivateMembers._

object PrivateMembersEnums {
  final val enum___privateEnum_A: enum___privateEnum = 0.toUInt
  final val enum___privateEnum_B: enum___privateEnum = 1.toUInt

  final val enum_enumWithPrivateMembers___C: enum_enumWithPrivateMembers = 0.toUInt
  final val enum_enumWithPrivateMembers_D: enum_enumWithPrivateMembers = 1.toUInt

  final val enum___E: native.CUnsignedInt = 0.toUInt
  final val enum_F: native.CUnsignedInt = 1.toUInt
}

object PrivateMembersHelpers {

  implicit class struct_structWithPrivateType_ops(val p: native.Ptr[struct_structWithPrivateType]) extends AnyVal {
    def field1: native.CInt = !p._1
    def field1_=(value: native.CInt):Unit = !p._1 = value
    def field2: __private_type = !p._2
    def field2_=(value: __private_type):Unit = !p._2 = value
  }

  def struct_structWithPrivateType()(implicit z: native.Zone): native.Ptr[struct_structWithPrivateType] = native.alloc[struct_structWithPrivateType]

  implicit class struct_structWithPrivateStruct_ops(val p: native.Ptr[struct_structWithPrivateStruct]) extends AnyVal {
    def s: native.Ptr[struct_structWithPrivateType] = !p._1
    def s_=(value: native.Ptr[struct_structWithPrivateType]):Unit = !p._1 = value
  }

  def struct_structWithPrivateStruct()(implicit z: native.Zone): native.Ptr[struct_structWithPrivateStruct] = native.alloc[struct_structWithPrivateStruct]

  implicit class struct_normalStruct_ops(val p: native.Ptr[struct_normalStruct]) extends AnyVal {
    def a: native.CInt = !p._1
    def a_=(value: native.CInt):Unit = !p._1 = value
  }

  def struct_normalStruct()(implicit z: native.Zone): native.Ptr[struct_normalStruct] = native.alloc[struct_normalStruct]

  implicit class struct_privateStructWithTypedef_ops(val p: native.Ptr[struct_privateStructWithTypedef]) extends AnyVal {
    def a: native.Ptr[__private_type] = !p._1
    def a_=(value: native.Ptr[__private_type]):Unit = !p._1 = value
  }

  def struct_privateStructWithTypedef()(implicit z: native.Zone): native.Ptr[struct_privateStructWithTypedef] = native.alloc[struct_privateStructWithTypedef]

  implicit class union___unionWithPrivateName_pos(val p: native.Ptr[union___unionWithPrivateName]) extends AnyVal {
    def a: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
    def a_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
  }
}
