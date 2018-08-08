package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object CustomNames {
  type page = native.CStruct2[native.CString, native.Ptr[Byte]]
  type book = native.CStruct1[native.Ptr[page]]
  type MY_INT = native.CInt
}
