package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object VarDefine {
  @name("a")
  def A: native.CInt = native.extern
  def getA(): native.CInt = native.extern
}
