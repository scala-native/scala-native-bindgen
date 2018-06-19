package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

object LiteralDefineDefines {
  val STRING: native.CString = c"Hello, World!"
  val LONG: native.CLong = 1000000000000L
  val LONG_WITHOUT_ENDING: native.CLong = 1000000000000L
  val LONG_LONG: native.CLongLong = 1000000000000L
  val MAXIMUM_SIGNED_LONG: native.CLong = 9223372036854775807L
  val MINIMUM_SIGNED_LONG: native.CLong = -9223372036854775808L
  val FLOAT: native.CDouble = 5.6
  val INT: native.CInt = 42
  val MAXIMUM_INT: native.CInt = 2147483647
  val NEW_INT: native.CInt = 42
  val NEG_INT: native.CInt = -42
  val SHOULD_BE_DEFINED: native.CString = c"Because INT is not equal to 0"
}
