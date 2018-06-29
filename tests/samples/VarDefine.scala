package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object VarDefine {
  val a: native.CInt = native.extern
  val constInt: native.CInt = native.extern
  val constIntPointer: native.Ptr[native.CInt] = native.extern
  val c: native.CInt = native.extern
  val f: native.CFloat = native.extern
  @name("a")
  val A: native.CInt = native.extern
  @name("constInt")
  val CONST_INT: native.CInt = native.extern
  @name("constIntPointer")
  val CONST_INT_POINTER: native.Ptr[native.CInt] = native.extern
}
