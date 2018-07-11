package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Cycles {
  type struct_b = native.CStruct1[native.Ptr[Byte]]
  type struct_a = native.CStruct1[native.Ptr[Byte]]
  type struct_c = native.CStruct1[struct_a]
}

import Cycles._

object CyclesHelpers {

  implicit class struct_a_ops(val p: native.Ptr[struct_a]) extends AnyVal {
    def bb: native.Ptr[struct_b] = (!p._1).cast[native.Ptr[struct_b]]
    def bb_=(value: native.Ptr[struct_b]): Unit = !p._1 = value.cast[native.Ptr[Byte]]
  }

  def struct_a()(implicit z: native.Zone): native.Ptr[struct_a] = native.alloc[struct_a]

  implicit class struct_b_ops(val p: native.Ptr[struct_b]) extends AnyVal {
    def cc: native.Ptr[struct_c] = (!p._1).cast[native.Ptr[struct_c]]
    def cc_=(value: native.Ptr[struct_c]): Unit = !p._1 = value.cast[native.Ptr[Byte]]
  }

  def struct_b()(implicit z: native.Zone): native.Ptr[struct_b] = native.alloc[struct_b]

  implicit class struct_c_ops(val p: native.Ptr[struct_c]) extends AnyVal {
    def aa: native.Ptr[struct_a] = p._1
    def aa_=(value: native.Ptr[struct_a]): Unit = !p._1 = !value
  }

  def struct_c()(implicit z: native.Zone): native.Ptr[struct_c] = native.alloc[struct_c]
}

