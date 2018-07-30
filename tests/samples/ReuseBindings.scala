package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object ReuseBindings {
  type aliasForBigStruct = org.scalanative.bindgen.samples.Struct.struct_bigStruct
  type struct_usesImportedEnum = native.CStruct1[org.scalanative.bindgen.samples.Struct.enum_pointIndex]
  def useStruct(anonymous0: native.Ptr[org.scalanative.bindgen.samples.Struct.struct_point]): Unit = native.extern
  def returnTypedef_point_s(): native.Ptr[org.scalanative.bindgen.samples.Struct.struct_point] = native.extern
  def returnTypedef_point(): native.Ptr[org.scalanative.bindgen.samples.Struct.point] = native.extern
  def readBook(book: native.Ptr[org.scalanative.bindgen.samples.CustomNames.book]): Unit = native.extern
  def getMyInt(): org.scalanative.bindgen.samples.CustomNames.MY_INT = native.extern
}

import ReuseBindings._

object ReuseBindingsHelpers {

  implicit class struct_usesImportedEnum_ops(val p: native.Ptr[struct_usesImportedEnum]) extends AnyVal {
    def index: org.scalanative.bindgen.samples.Struct.enum_pointIndex = !p._1
    def index_=(value: org.scalanative.bindgen.samples.Struct.enum_pointIndex): Unit = !p._1 = value
  }

  def struct_usesImportedEnum()(implicit z: native.Zone): native.Ptr[struct_usesImportedEnum] = native.alloc[struct_usesImportedEnum]
}

