package org.scalanative.bindgen.bindings.posix

import scala.scalanative._
import scala.scalanative.native._

@native.extern
object fnmatch {
  def fnmatch(__pattern: native.CString, __name: native.CString, __flags: native.CInt): native.CInt = native.extern

  object defines {
    val _FNMATCH_H: native.CInt = 1
    val FNM_NOMATCH: native.CInt = 1
  }
}
