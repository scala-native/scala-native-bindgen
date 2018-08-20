package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object ReuseBindings {
  type aliasForBigStruct = org.scalanative.bindgen.samples.Struct.struct_bigStruct
  type struct_usesImportedEnum = native.CStruct2[org.scalanative.bindgen.samples.Struct.enum_pointIndex, org.scalanative.bindgen.samples.CustomNames.weight]
  def useStruct(p0: native.Ptr[org.scalanative.bindgen.samples.Struct.struct_point]): Unit = native.extern
  def returnTypedef_point_s(): native.Ptr[org.scalanative.bindgen.samples.Struct.struct_point] = native.extern
  def returnTypedef_point(): native.Ptr[org.scalanative.bindgen.samples.Struct.point] = native.extern
  def readBook(book: native.Ptr[org.scalanative.bindgen.samples.CustomNames.book]): Unit = native.extern
  def getWeight(weight: native.Ptr[org.scalanative.bindgen.samples.CustomNames.weight]): Unit = native.extern
  def getMyInt(): org.scalanative.bindgen.samples.CustomNames.MY_INT = native.extern
  def getEnum(): org.scalanative.bindgen.samples.CustomNames.EnumWithTypedef = native.extern

  object implicits {
    implicit class struct_usesImportedEnum_ops(val p: native.Ptr[struct_usesImportedEnum]) extends AnyVal {
      def index: org.scalanative.bindgen.samples.Struct.enum_pointIndex = !p._1
      def index_=(value: org.scalanative.bindgen.samples.Struct.enum_pointIndex): Unit = !p._1 = value
      def weight: native.Ptr[org.scalanative.bindgen.samples.CustomNames.weight] = p._2
      def weight_=(value: native.Ptr[org.scalanative.bindgen.samples.CustomNames.weight]): Unit = !p._2 = !value
    }
  }

  object struct_usesImportedEnum {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_usesImportedEnum] = native.alloc[struct_usesImportedEnum]
    def apply(index: org.scalanative.bindgen.samples.Struct.enum_pointIndex, weight: native.Ptr[org.scalanative.bindgen.samples.CustomNames.weight])(implicit z: native.Zone): native.Ptr[struct_usesImportedEnum] = {
      val ptr = native.alloc[struct_usesImportedEnum]
      ptr.index = index
      ptr.weight = weight
      ptr
    }
  }
}
