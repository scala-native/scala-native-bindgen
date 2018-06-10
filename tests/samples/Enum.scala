import scala.scalanative._
import scala.scalanative.native._

@native.link("Enum")
@native.extern
object Enum {
  type enum_days = native.CUnsignedInt
  type enum_bigValues = native.CUnsignedLong
  type enum_negativeValues = native.CInt
  type enum_bigNegativeValues = native.CLong
}

import Enum._

object EnumEnums {
  final val enum_days_MONDAY: enum_days = 0.toUInt
  final val enum_days_TUESDAY: enum_days = 200.toUInt
  final val enum_days_WEDNESDAY: enum_days = 201.toUInt
  final val enum_days_THURSDAY: enum_days = 4.toUInt
  final val enum_days_FRIDAY: enum_days = 5.toUInt
  final val enum_days_SATURDAY: enum_days = 3.toUInt
  final val enum_days_SUNDAY: enum_days = 4.toUInt

  final val enum_bigValues_BIG_A: enum_bigValues = 10000000000L.toULong
  final val enum_bigValues_BIG_B: enum_bigValues = 1L.toULong

  final val enum_ANON_A: native.CUnsignedInt = 0.toUInt
  final val enum_ANON_B: native.CUnsignedInt = 1.toUInt

  final val enum_negativeValues_NEG_A: enum_negativeValues = -1
  final val enum_negativeValues_NEG_B: enum_negativeValues = -2

  final val enum_bigNegativeValues_BIG_NEG_A: enum_bigNegativeValues = -10000000000L
  final val enum_bigNegativeValues_BIG_NEG_B: enum_bigNegativeValues = -1L
}
