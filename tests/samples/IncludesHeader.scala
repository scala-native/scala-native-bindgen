package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object IncludesHeader {
  type enum_semester = native.CUnsignedInt
  object enum_semester {
    final val AUTUMN: enum_semester = 0.toUInt
    final val SPRING: enum_semester = 1.toUInt
  }

  type size = native.CInt
  type struct_metadata = native.CStruct2[native.CUnsignedInt, native.CString]
  type metadata = struct_metadata
  type struct_document = native.CStruct1[metadata]
  type struct_courseInfo = native.CStruct2[native.CString, enum_semester]
  def getSize(d: native.Ptr[struct_document]): size = native.extern
}

import IncludesHeader._

object IncludesHeaderHelpers {

  implicit class struct_metadata_ops(val p: native.Ptr[struct_metadata]) extends AnyVal {
    def year: native.CUnsignedInt = !p._1
    def year_=(value: native.CUnsignedInt): Unit = !p._1 = value
    def publisher: native.CString = !p._2
    def publisher_=(value: native.CString): Unit = !p._2 = value
  }

  def struct_metadata()(implicit z: native.Zone): native.Ptr[struct_metadata] = native.alloc[struct_metadata]

  implicit class struct_document_ops(val p: native.Ptr[struct_document]) extends AnyVal {
    def m: native.Ptr[metadata] = p._1
    def m_=(value: native.Ptr[metadata]): Unit = !p._1 = !value
  }

  def struct_document()(implicit z: native.Zone): native.Ptr[struct_document] = native.alloc[struct_document]

  implicit class struct_courseInfo_ops(val p: native.Ptr[struct_courseInfo]) extends AnyVal {
    def name: native.CString = !p._1
    def name_=(value: native.CString): Unit = !p._1 = value
    def s: enum_semester = !p._2
    def s_=(value: enum_semester): Unit = !p._2 = value
  }

  def struct_courseInfo()(implicit z: native.Zone): native.Ptr[struct_courseInfo] = native.alloc[struct_courseInfo]
}
