package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Union {
  type union_values = native.CArray[Byte, native.Nat._8]
  def setIntValue(v: native.Ptr[union_values]): Unit = native.extern
  def setLongValue(v: native.Ptr[union_values]): Unit = native.extern
}

import Union._

object UnionHelpers {

  implicit class union_values_pos(val p: native.Ptr[union_values]) extends AnyVal {
    def l: native.Ptr[native.CLong] = p.cast[native.Ptr[native.CLong]]
    def l_=(value: native.CLong): Unit = !p.cast[native.Ptr[native.CLong]] = value
    def i: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
    def i_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    def ll: native.Ptr[native.CLongLong] = p.cast[native.Ptr[native.CLongLong]]
    def ll_=(value: native.CLongLong): Unit = !p.cast[native.Ptr[native.CLongLong]] = value
  }
}
