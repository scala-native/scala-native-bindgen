import scala.scalanative._
import scala.scalanative.native._
import scala.scalanative.native.Nat._

@native.link("Enum")
@native.extern
object Enum {
  type enum_days = native.CUnsignedInt
  type enum_bigValues = native.CUnsignedLong
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

  final val enum_bigValues_A: enum_bigValues = 10000000000L.toULong
  final val enum_bigValues_B: enum_bigValues = 1L.toULong

  final val enum_C: native.CUnsignedInt = 0.toUInt
  final val enum_D: native.CUnsignedInt = 1.toUInt
}
