package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object nativeLib {
  type nativeFunc0 = native.CInt
  @native.link("native")
  def nativeFunc0(p0: native.CInt): Unit = native.extern
  def nativeFunc(p0: native.CFloat): Unit = native.extern
}
