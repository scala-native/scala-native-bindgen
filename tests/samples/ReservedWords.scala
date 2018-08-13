package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object ReservedWords {
  type `match` = native.CInt
  type `var` = native.CArray[`match`, native.Nat._5]
  type struct_object = native.CStruct2[`match`, native.CInt]
  type `object` = struct_object
  type `type` = struct_object
  type struct_anonymous_0 = native.CStruct2[native.CChar, native.Ptr[`type`]]
  type union_lazy = native.CArray[Byte, native.Nat._8]
  type `lazy` = union_lazy
  type `def` = `match`
  type struct_finally = native.CStruct2[`def`, `lazy`]
  type `finally` = struct_finally
  def `with`(`sealed`: `match`, `implicit`: native.Ptr[`match`], `forSome`: native.Ptr[`lazy`]): native.Ptr[`type`] = native.extern
  def `implicit`(`type`: native.Ptr[`finally`]): `match` = native.extern
  def _1(): Unit = native.extern

  object implicits {
    implicit class struct_object_ops(val p: native.Ptr[struct_object]) extends AnyVal {
      def `yield`: `match` = !p._1
      def `yield_=`(value: `match`): Unit = !p._1 = value
      def `val`: native.CInt = !p._2
      def `val_=`(value: native.CInt): Unit = !p._2 = value
    }

    implicit class struct_anonymous_0_ops(val p: native.Ptr[struct_anonymous_0]) extends AnyVal {
      def `def`: native.CChar = !p._1
      def `def_=`(value: native.CChar): Unit = !p._1 = value
      def `super`: native.Ptr[`type`] = !p._2
      def `super_=`(value: native.Ptr[`type`]): Unit = !p._2 = value
    }

    implicit class struct_finally_ops(val p: native.Ptr[struct_finally]) extends AnyVal {
      def `val`: `def` = !p._1
      def `val_=`(value: `def`): Unit = !p._1 = value
      def `finally`: native.Ptr[`lazy`] = p._2
      def `finally_=`(value: native.Ptr[`lazy`]): Unit = !p._2 = !value
    }

    implicit class union_lazy_pos(val p: native.Ptr[union_lazy]) extends AnyVal {
      def instance: native.Ptr[native.Ptr[`object`]] = p.cast[native.Ptr[native.Ptr[`object`]]]
      def instance_=(value: native.Ptr[`object`]): Unit = !p.cast[native.Ptr[native.Ptr[`object`]]] = value
      def `forSome`: native.Ptr[`match`] = p.cast[native.Ptr[`match`]]
      def `forSome_=`(value: `match`): Unit = !p.cast[native.Ptr[`match`]] = value
      def `implicit`: native.Ptr[native.Ptr[struct_anonymous_0]] = p.cast[native.Ptr[native.Ptr[struct_anonymous_0]]]
      def `implicit_=`(value: native.Ptr[struct_anonymous_0]): Unit = !p.cast[native.Ptr[native.Ptr[struct_anonymous_0]]] = value
    }
  }

  object struct_object {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_object] = native.alloc[struct_object]
    def apply(`yield`: `match`, `val`: native.CInt)(implicit z: native.Zone): native.Ptr[struct_object] = {
      val ptr = native.alloc[struct_object]
      ptr.`yield` = `yield`
      ptr.`val` = `val`
      ptr
    }
  }

  object struct_anonymous_0 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_anonymous_0] = native.alloc[struct_anonymous_0]
    def apply(`def`: native.CChar, `super`: native.Ptr[`type`])(implicit z: native.Zone): native.Ptr[struct_anonymous_0] = {
      val ptr = native.alloc[struct_anonymous_0]
      ptr.`def` = `def`
      ptr.`super` = `super`
      ptr
    }
  }

  object struct_finally {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_finally] = native.alloc[struct_finally]
    def apply(`val`: `def`, `finally`: native.Ptr[`lazy`])(implicit z: native.Zone): native.Ptr[struct_finally] = {
      val ptr = native.alloc[struct_finally]
      ptr.`val` = `val`
      ptr.`finally` = `finally`
      ptr
    }
  }
}
