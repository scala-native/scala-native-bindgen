package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

object Cycles {
  type struct_node = native.CStruct2[native.CInt, native.Ptr[Byte]]
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

  object implicits {
    implicit class struct_node_ops(val p: native.Ptr[struct_node]) extends AnyVal {
      def value: native.CInt = !p._1
      def value_=(value: native.CInt): Unit = !p._1 = value
      def next: native.Ptr[struct_node] = (!p._2).cast[native.Ptr[struct_node]]
      def next_=(value: native.Ptr[struct_node]): Unit = !p._2 = value.cast[native.Ptr[Byte]]
    }

    implicit class struct_a_ops(val p: native.Ptr[struct_a]) extends AnyVal {
      def bb: native.Ptr[struct_b] = !p._1
      def bb_=(value: native.Ptr[struct_b]): Unit = !p._1 = value
    }

    implicit class struct_b_ops(val p: native.Ptr[struct_b]) extends AnyVal {
      def cc: native.Ptr[native.Ptr[struct_c]] = (!p._1).cast[native.Ptr[native.Ptr[struct_c]]]
      def cc_=(value: native.Ptr[native.Ptr[struct_c]]): Unit = !p._1 = value.cast[native.Ptr[native.Ptr[Byte]]]
    }

    implicit class struct_c_ops(val p: native.Ptr[struct_c]) extends AnyVal {
      def aa: native.Ptr[struct_a] = p._1
      def aa_=(value: native.Ptr[struct_a]): Unit = !p._1 = !value
    }

    implicit class struct_FuncPointerCycle1_ops(val p: native.Ptr[struct_FuncPointerCycle1]) extends AnyVal {
      def s: native.Ptr[struct_FuncPointerCycle2] = !p._1
      def s_=(value: native.Ptr[struct_FuncPointerCycle2]): Unit = !p._1 = value
    }

    implicit class struct_FuncPointerCycle2_ops(val p: native.Ptr[struct_FuncPointerCycle2]) extends AnyVal {
      def memberFunction: native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]] = (!p._1).cast[native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]]]
      def memberFunction_=(value: native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]]): Unit = !p._1 = value.cast[native.CFunctionPtr0[native.Ptr[Byte]]]
    }

    implicit class struct_TypeWithTypedef2_ops(val p: native.Ptr[struct_TypeWithTypedef2]) extends AnyVal {
      def s: native.Ptr[TypeWithTypedef1] = (!p._1).cast[native.Ptr[TypeWithTypedef1]]
      def s_=(value: native.Ptr[TypeWithTypedef1]): Unit = !p._1 = value.cast[native.Ptr[Byte]]
    }

    implicit class struct_TypeWithTypedef1_ops(val p: native.Ptr[struct_TypeWithTypedef1]) extends AnyVal {
      def s: native.Ptr[struct_TypeWithTypedef2] = !p._1
      def s_=(value: native.Ptr[struct_TypeWithTypedef2]): Unit = !p._1 = value
    }

    implicit class struct_TwoTypesReplaced3_ops(val p: native.Ptr[struct_TwoTypesReplaced3]) extends AnyVal {
      def memberFunction: native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]] = (!p._1).cast[native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]]]
      def memberFunction_=(value: native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]]): Unit = !p._1 = value.cast[native.CFunctionPtr1[native.Ptr[Byte], native.Ptr[Byte]]]
    }

    implicit class struct_TwoTypesReplaced1_ops(val p: native.Ptr[struct_TwoTypesReplaced1]) extends AnyVal {
      def s: native.Ptr[struct_TwoTypesReplaced2] = !p._1
      def s_=(value: native.Ptr[struct_TwoTypesReplaced2]): Unit = !p._1 = value
    }

    implicit class struct_TwoTypesReplaced2_ops(val p: native.Ptr[struct_TwoTypesReplaced2]) extends AnyVal {
      def s: native.Ptr[struct_TwoTypesReplaced3] = !p._1
      def s_=(value: native.Ptr[struct_TwoTypesReplaced3]): Unit = !p._1 = value
    }

    implicit class struct_cycleWithUnionS_ops(val p: native.Ptr[struct_cycleWithUnionS]) extends AnyVal {
      def u: native.Ptr[union_cycleWithUnionU] = !p._1
      def u_=(value: native.Ptr[union_cycleWithUnionU]): Unit = !p._1 = value
    }

    implicit class struct_FuncPointerWithValueType1_ops(val p: native.Ptr[struct_FuncPointerWithValueType1]) extends AnyVal {
      def s: native.Ptr[struct_FuncPointerWithValueType2] = !p._1
      def s_=(value: native.Ptr[struct_FuncPointerWithValueType2]): Unit = !p._1 = value
    }

    implicit class struct_FuncPointerWithValueType2_ops(val p: native.Ptr[struct_FuncPointerWithValueType2]) extends AnyVal {
      def memberFunction: native.CFunctionPtr0[struct_FuncPointerWithValueType1] = (!p._1).cast[native.CFunctionPtr0[struct_FuncPointerWithValueType1]]
      def memberFunction_=(value: native.CFunctionPtr0[struct_FuncPointerWithValueType1]): Unit = !p._1 = value.cast[native.CFunctionPtr0[native.CStruct0]]
    }

    implicit class union_cycleWithUnionU_pos(val p: native.Ptr[union_cycleWithUnionU]) extends AnyVal {
      def s: native.Ptr[native.Ptr[struct_cycleWithUnionS]] = p.cast[native.Ptr[native.Ptr[struct_cycleWithUnionS]]]
      def s_=(value: native.Ptr[struct_cycleWithUnionS]): Unit = !p.cast[native.Ptr[native.Ptr[struct_cycleWithUnionS]]] = value
    }
  }

  object struct_node {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_node] = native.alloc[struct_node]
    def apply(value: native.CInt, next: native.Ptr[struct_node])(implicit z: native.Zone): native.Ptr[struct_node] = {
      val ptr = native.alloc[struct_node]
      ptr.value = value
      ptr.next = next
      ptr
    }
  }

  object struct_a {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_a] = native.alloc[struct_a]
    def apply(bb: native.Ptr[struct_b])(implicit z: native.Zone): native.Ptr[struct_a] = {
      val ptr = native.alloc[struct_a]
      ptr.bb = bb
      ptr
    }
  }

  object struct_b {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_b] = native.alloc[struct_b]
    def apply(cc: native.Ptr[native.Ptr[struct_c]])(implicit z: native.Zone): native.Ptr[struct_b] = {
      val ptr = native.alloc[struct_b]
      ptr.cc = cc
      ptr
    }
  }

  object struct_c {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_c] = native.alloc[struct_c]
    def apply(aa: native.Ptr[struct_a])(implicit z: native.Zone): native.Ptr[struct_c] = {
      val ptr = native.alloc[struct_c]
      ptr.aa = aa
      ptr
    }
  }

  object struct_FuncPointerCycle1 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_FuncPointerCycle1] = native.alloc[struct_FuncPointerCycle1]
    def apply(s: native.Ptr[struct_FuncPointerCycle2])(implicit z: native.Zone): native.Ptr[struct_FuncPointerCycle1] = {
      val ptr = native.alloc[struct_FuncPointerCycle1]
      ptr.s = s
      ptr
    }
  }

  object struct_FuncPointerCycle2 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_FuncPointerCycle2] = native.alloc[struct_FuncPointerCycle2]
    def apply(memberFunction: native.CFunctionPtr0[native.Ptr[struct_FuncPointerCycle1]])(implicit z: native.Zone): native.Ptr[struct_FuncPointerCycle2] = {
      val ptr = native.alloc[struct_FuncPointerCycle2]
      ptr.memberFunction = memberFunction
      ptr
    }
  }

  object struct_TypeWithTypedef2 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_TypeWithTypedef2] = native.alloc[struct_TypeWithTypedef2]
    def apply(s: native.Ptr[TypeWithTypedef1])(implicit z: native.Zone): native.Ptr[struct_TypeWithTypedef2] = {
      val ptr = native.alloc[struct_TypeWithTypedef2]
      ptr.s = s
      ptr
    }
  }

  object struct_TypeWithTypedef1 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_TypeWithTypedef1] = native.alloc[struct_TypeWithTypedef1]
    def apply(s: native.Ptr[struct_TypeWithTypedef2])(implicit z: native.Zone): native.Ptr[struct_TypeWithTypedef1] = {
      val ptr = native.alloc[struct_TypeWithTypedef1]
      ptr.s = s
      ptr
    }
  }

  object struct_TwoTypesReplaced3 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced3] = native.alloc[struct_TwoTypesReplaced3]
    def apply(memberFunction: native.CFunctionPtr1[native.Ptr[struct_TwoTypesReplaced2], native.Ptr[struct_TwoTypesReplaced1]])(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced3] = {
      val ptr = native.alloc[struct_TwoTypesReplaced3]
      ptr.memberFunction = memberFunction
      ptr
    }
  }

  object struct_TwoTypesReplaced1 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced1] = native.alloc[struct_TwoTypesReplaced1]
    def apply(s: native.Ptr[struct_TwoTypesReplaced2])(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced1] = {
      val ptr = native.alloc[struct_TwoTypesReplaced1]
      ptr.s = s
      ptr
    }
  }

  object struct_TwoTypesReplaced2 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced2] = native.alloc[struct_TwoTypesReplaced2]
    def apply(s: native.Ptr[struct_TwoTypesReplaced3])(implicit z: native.Zone): native.Ptr[struct_TwoTypesReplaced2] = {
      val ptr = native.alloc[struct_TwoTypesReplaced2]
      ptr.s = s
      ptr
    }
  }

  object struct_cycleWithUnionS {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_cycleWithUnionS] = native.alloc[struct_cycleWithUnionS]
    def apply(u: native.Ptr[union_cycleWithUnionU])(implicit z: native.Zone): native.Ptr[struct_cycleWithUnionS] = {
      val ptr = native.alloc[struct_cycleWithUnionS]
      ptr.u = u
      ptr
    }
  }

  object struct_FuncPointerWithValueType1 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_FuncPointerWithValueType1] = native.alloc[struct_FuncPointerWithValueType1]
    def apply(s: native.Ptr[struct_FuncPointerWithValueType2])(implicit z: native.Zone): native.Ptr[struct_FuncPointerWithValueType1] = {
      val ptr = native.alloc[struct_FuncPointerWithValueType1]
      ptr.s = s
      ptr
    }
  }

  object struct_FuncPointerWithValueType2 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_FuncPointerWithValueType2] = native.alloc[struct_FuncPointerWithValueType2]
    def apply(memberFunction: native.CFunctionPtr0[struct_FuncPointerWithValueType1])(implicit z: native.Zone): native.Ptr[struct_FuncPointerWithValueType2] = {
      val ptr = native.alloc[struct_FuncPointerWithValueType2]
      ptr.memberFunction = memberFunction
      ptr
    }
  }
}
