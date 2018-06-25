package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object VarDefine {
  @name("a")
  val A: native.CInt = native.extern
}
