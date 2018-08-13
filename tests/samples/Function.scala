package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Function {
  type struct_s = native.CStruct1[native.CInt]
  type s = struct_s
  type union_u = native.CArray[Byte, native.Nat._4]
  def no_args(): native.CInt = native.extern
  def void_arg(): native.CFloat = native.extern
  def one_arg(a: native.CInt): native.CChar = native.extern
  def two_args(a: native.CFloat, b: native.CInt): native.Ptr[Byte] = native.extern
  def anonymous_args(p0: native.CFloat, p1: native.CInt): native.CDouble = native.extern
  def variadic_args(a: native.CDouble, varArgs: native.CString, varArgs0: native.CVararg*): native.CDouble = native.extern
  def acceptsArray(p0: native.Ptr[native.CInt]): Unit = native.extern

  object implicits {
    implicit class struct_s_ops(val p: native.Ptr[struct_s]) extends AnyVal {
      def `val`: native.CInt = !p._1
      def `val_=`(value: native.CInt): Unit = !p._1 = value
    }

    implicit class union_u_pos(val p: native.Ptr[union_u]) extends AnyVal {
      def a: native.Ptr[native.CInt] = p.cast[native.Ptr[native.CInt]]
      def a_=(value: native.CInt): Unit = !p.cast[native.Ptr[native.CInt]] = value
    }
  }

  object struct_s {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_s] = native.alloc[struct_s]
    def apply(`val`: native.CInt)(implicit z: native.Zone): native.Ptr[struct_s] = {
      val ptr = native.alloc[struct_s]
      ptr.`val` = `val`
      ptr
    }
  }
}
