import scala.scalanative._
import scala.scalanative.native._
import scala.scalanative.native.Nat._

@native.link("Typedef")
@native.extern
object Typedef {
  type toggle_e = enum_toggle_e
  type int2int = native.CFunctionPtr1[native.CInt, native.CInt]
  type day2string = native.CFunctionPtr1[enum_days, native.CString]
  type toggle = native.CFunctionPtr1[toggle_e, Unit]
  type enum_days = native.CUnsignedInt
  type enum_toggle_e = native.CUnsignedInt
}

import Typedef._

object TypedefEnums {
  final val enum_days_MONDAY: enum_days = 0.toUInt
  final val enum_days_TUESDAY: enum_days = 1.toUInt
  final val enum_days_WEDNESDAY: enum_days = 2.toUInt
  final val enum_days_THURSDAY: enum_days = 3.toUInt
  final val enum_days_FRIDAY: enum_days = 4.toUInt
  final val enum_days_SATURDAY: enum_days = 5.toUInt
  final val enum_days_SUNDAY: enum_days = 6.toUInt

  final val enum_toggle_e_OFF: enum_toggle_e = 0.toUInt
  final val enum_toggle_e_ON: enum_toggle_e = 1.toUInt
}
