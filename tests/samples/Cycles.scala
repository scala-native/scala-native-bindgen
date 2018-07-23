package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Cycles {
  type struct_b = native.CStruct1[native.Ptr[native.Ptr[Byte]]]
  type struct_a = native.CStruct1[native.Ptr[struct_b]]
  type struct_c = native.CStruct1[struct_a]
  type struct_FuncPointerCycle2 = native.CStruct1[native.CFunctionPtr0[native.Ptr[Byte]]]
  type struct_FuncPointerCycle1 = native.CStruct1[native.Ptr[struct_FuncPointerCycle2]]
  type struct_TypeWithTypedef1 = native.CStruct1[native.Ptr[struct_TypeWithTypedef2]]
  type TypeWithTypedef1 = struct_TypeWithTypedef1
  type struct_TypeWithTypedef2 = native.CStruct1[native.Ptr[Byte]]
  type struct_TwoTypesReplaced1 = native.CStruct1[native.Ptr[struct_TwoTypesReplaced2]]
  type struct_TwoTypesReplaced2 = native.CStruct1[native.Ptr[struct_TwoTypesReplaced3]]
  type struct_TwoTypesReplaced3 = native.CStruct1[native.CFunctionPtr1[native.Ptr[Byte], native.Ptr[Byte]]]
  type union_cycleWithUnionU = native.CArray[Byte, native.Nat._8]
  type struct_cycleWithUnionS = native.CStruct1[native.Ptr[union_cycleWithUnionU]]
  type struct_FuncPointerWithValueType2 = native.CStruct1[native.CFunctionPtr0[native.CStruct0]]
  type struct_FuncPointerWithValueType1 = native.CStruct1[native.Ptr[struct_FuncPointerWithValueType2]]
}

import Cycles._

object CyclesHelpers {

  implicit class struct_a_ops(val p: native.Ptr[struct_a]) extends AnyVal {
    def bb: native.Ptr[struct_b] = !p._1
    def bb_=(value: native.Ptr[struct_b]): Unit = !p._1 = value
  }

  def struct_a()(implicit z: native.Zone): native.Ptr[struct_a] = native.alloc[struct_a]

  implicit class struct_b_ops(val p: native.Ptr[struct_b]) extends AnyVal {
    def cc: native.Ptr[native.Ptr[struct_c]] = (!p._1).cast[native.Ptr[native.Ptr[struct_c]]]
    def cc_=(value: native.Ptr[native.Ptr[struct_c]]): Unit = !p._1 = value.cast[native.Ptr[native.Ptr[Byte]]]
  }

  def struct_b()(implicit z: native.Zone): native.Ptr[struct_b] = native.alloc[struct_b]

  implicit class struct_c_ops(val p: native.Ptr[struct_c]) extends AnyVal {
    def aa: native.Ptr[struct_a] = p._1
    def aa_=(value: native.Ptr[struct_a]): Unit = !p._1 = !value
  }

  def struct_c()(implicit z: native.Zone): native.Ptr[struct_c] = native.alloc[struct_c]

  implicit class struct_FuncPointerCycle1_ops(val p: native.Ptr[struct_FuncPointerCycle1]) extends AnyVal {
    def s: native.Ptr[struct_FuncPointerCycle2] = !p._1
    def s_=(value: native.Ptr[struct_FuncPointerCycle2]): Unit = !p._1 = value
  }

  def struct_FuncPointerCycle1()(implicit z: native.Zone): native.Ptr[struct_FuncPointerCycle1] = native.alloc[struct_FuncPointerCycle1]

  implicit class struct_FuncPointerCycle2_ops(val p: native.Ptr[struct_FuncPointerCycle2]) extends AnyVal {
    def memberFunction: native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]] = (!p._1).cast[native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]]]
    def memberFunction_=(value: native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]]): Unit = !p._1 = value.cast[native.CFunctionPtr0[native.Ptr[Byte]]]
  }

  def struct_FuncPointerCycle2()(implicit z: native.Zone): native.Ptr[struct_FuncPointerCycle2] = native.alloc[struct_FuncPointerCycle2]

  implicit class struct_TypeWithTypedef2_ops(val p: native.Ptr[struct_TypeWithTypedef2]) extends AnyVal {
    def s: native.Ptr[TypeWithTypedef1] = (!p._1).cast[native.Ptr[TypeWithTypedef1]]
    def s_=(value: native.Ptr[TypeWithTypedef1]): Unit = !p._1 = value.cast[native.Ptr[Byte]]
  }

  def struct_TypeWithTypedef2()(implicit z: native.Zone): native.Ptr[struct_TypeWithTypedef2] = native.alloc[struct_TypeWithTypedef2]

  implicit class struct_TypeWithTypedef1_ops(val p: native.Ptr[struct_TypeWithTypedef1]) extends AnyVal {
    def s: native.Ptr[struct_TypeWithTypedef2] = !p._1
    def s_=(value: native.Ptr[struct_TypeWithTypedef2]): Unit = !p._1 = value
  }

  def struct_TypeWithTypedef1()(implicit z: native.Zone): native.Ptr[struct_TypeWithTypedef1] = native.alloc[struct_TypeWithTypedef1]

  implicit class struct_TwoTypesReplaced3_ops(val p: native.Ptr[struct_TwoTypesReplaced3]) extends AnyVal {
    def memberFunction: native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]] = (!p._1).cast[native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]]]
    def memberFunction_=(value: native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]]): Unit = !p._1 = value.cast[native.CFunctionPtr1[native.Ptr[Byte], native.Ptr[Byte]]]
  }

  def struct_TwoTypesReplaced3()(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced3] = native.alloc[struct_TwoTypesReplaced3]

  implicit class struct_TwoTypesReplaced1_ops(val p: native.Ptr[struct_TwoTypesReplaced1]) extends AnyVal {
    def s: native.Ptr[struct_TwoTypesReplaced2] = !p._1
    def s_=(value: native.Ptr[struct_TwoTypesReplaced2]): Unit = !p._1 = value
  }

  def struct_TwoTypesReplaced1()(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced1] = native.alloc[struct_TwoTypesReplaced1]

  implicit class struct_TwoTypesReplaced2_ops(val p: native.Ptr[struct_TwoTypesReplaced2]) extends AnyVal {
    def s: native.Ptr[struct_TwoTypesReplaced3] = !p._1
    def s_=(value: native.Ptr[struct_TwoTypesReplaced3]): Unit = !p._1 = value
  }

  def struct_TwoTypesReplaced2()(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced2] = native.alloc[struct_TwoTypesReplaced2]

  implicit class struct_cycleWithUnionS_ops(val p: native.Ptr[struct_cycleWithUnionS]) extends AnyVal {
    def u: native.Ptr[union_cycleWithUnionU] = !p._1
    def u_=(value: native.Ptr[union_cycleWithUnionU]): Unit = !p._1 = value
  }

  def struct_cycleWithUnionS()(implicit z: native.Zone): native.Ptr[struct_cycleWithUnionS] = native.alloc[struct_cycleWithUnionS]

  implicit class struct_FuncPointerWithValueType1_ops(val p: native.Ptr[struct_FuncPointerWithValueType1]) extends AnyVal {
    def s: native.Ptr[struct_FuncPointerWithValueType2] = !p._1
    def s_=(value: native.Ptr[struct_FuncPointerWithValueType2]): Unit = !p._1 = value
  }

  def struct_FuncPointerWithValueType1()(implicit z: native.Zone): native.Ptr[struct_FuncPointerWithValueType1] = native.alloc[struct_FuncPointerWithValueType1]

  implicit class struct_FuncPointerWithValueType2_ops(val p: native.Ptr[struct_FuncPointerWithValueType2]) extends AnyVal {
    def memberFunction: native.CFunctionPtr0[struct_FuncPointerWithValueType1] = (!p._1).cast[native.CFunctionPtr0[struct_FuncPointerWithValueType1]]
    def memberFunction_=(value: native.CFunctionPtr0[struct_FuncPointerWithValueType1]): Unit = !p._1 = value.cast[native.CFunctionPtr0[native.CStruct0]]
  }

  def struct_FuncPointerWithValueType2()(implicit z: native.Zone): native.Ptr[struct_FuncPointerWithValueType2] = native.alloc[struct_FuncPointerWithValueType2]

  implicit class union_cycleWithUnionU_pos(val p: native.Ptr[union_cycleWithUnionU]) extends AnyVal {
    def s: native.Ptr[native.Ptr[struct_cycleWithUnionS]] = p.cast[native.Ptr[native.Ptr[struct_cycleWithUnionS]]]
    def s_=(value: native.Ptr[struct_cycleWithUnionS]): Unit = !p.cast[native.Ptr[native.Ptr[struct_cycleWithUnionS]]] = value
  }
}

