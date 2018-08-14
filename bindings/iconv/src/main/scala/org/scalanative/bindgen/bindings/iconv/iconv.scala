package org.scalanative.bindgen.bindings.iconv

import scala.scalanative._
import scala.scalanative.native._

@native.extern
object iconv {
  type iconv_t = native.Ptr[Byte]
  def iconv_open(__tocode: native.CString, __fromcode: native.CString): native.Ptr[Byte] = native.extern
  def iconv(__cd: native.Ptr[Byte], __inbuf: native.Ptr[native.CString], __inbytesleft: native.Ptr[native.CSize], __outbuf: native.Ptr[native.CString], __outbytesleft: native.Ptr[native.CSize]): native.CSize = native.extern
  def iconv_close(__cd: native.Ptr[Byte]): native.CInt = native.extern

  object defines {
    val _ICONV_H: native.CInt = 1
  }
}
