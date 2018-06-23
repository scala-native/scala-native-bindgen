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
  val OCTAL: native.CInt = 139
  val HEXADECIMAL: native.CInt = 75
  val EXPONENT: native.CDouble = 1e-10
  val DOT_EXPONENT: native.CDouble = 0.01
  val HEXADECIMAL_WITHOUT_RADIX: native.CDouble = 523264
  val HEXADECIMAL_WITH_RADIX: native.CDouble = 7.5
  val HEXADECIMAL_FRACTIONAL_WITH_RADIX: native.CDouble = 0.0355225
}
