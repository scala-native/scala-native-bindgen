package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object CustomNames {
  type enum_enumWithTypedef = native.CUnsignedInt
  object enum_enumWithTypedef {
    final val CONST: enum_enumWithTypedef = 0.toUInt
  }

  type EnumWithTypedef = enum_enumWithTypedef
  type page = native.CStruct2[native.CString, native.Ptr[Byte]]
  type book = native.CStruct1[native.Ptr[page]]
  type weight = native.CArray[Byte, native.Nat._4]
  type MY_INT = native.CInt
}
