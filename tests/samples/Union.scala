package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Union {
  type union_values = native.CArray[Byte, native.Nat._8]
  def getValues(): native.Ptr[union_values] = native.extern
}

import Union._

object UnionHelpers {

  implicit class union_values_pos(val p: native.Ptr[union_values]) extends AnyVal {
    def a: native.Ptr[native.CLong] = p.cast[native.Ptr[native.CLong]]
    def a_=(value: native.CLong): Unit = !p.cast[native.Ptr[native.CLong]] = value
    def b: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
    def b_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    def c: native.Ptr[native.CLongLong] = p.cast[native.Ptr[native.CLongLong]]
    def c_=(value: native.CLongLong): Unit = !p.cast[native.Ptr[native.CLongLong]] = value
  }
}
