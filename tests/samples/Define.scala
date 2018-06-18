package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

object DefineDefines {
  val STRING: native.CString = c"Hello, World!"
  val LONG: native.CLong = 1000000000000L
  val LONG_WITHOUT_ENDING: native.CLong = 1000000000000L
  val LONG_LONG: native.CLongLong = 1000000000000L
  val MAXIMUM_SIGNED_LONG: native.CLong = 9223372036854775807L
  val INT: native.CInt = 42
  val NEW_INT: native.CInt = 42
  val SHOULD_BE_DEFINED: native.CString = c"Because INT is not equal to 0"
}
