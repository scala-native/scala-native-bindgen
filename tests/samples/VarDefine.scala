package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object VarDefine {
  @name("a")
  val A: native.CInt = native.extern
  @name("constInt")
  val CONST_INT: native.CInt = native.extern
  @name("constIntPointer")
  val CONST_INT_POINTER: native.Ptr[native.CInt] = native.extern
}
