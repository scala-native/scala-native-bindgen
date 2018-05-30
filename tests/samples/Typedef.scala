import scala.scalanative._
import scala.scalanative.native._
import scala.scalanative.native.Nat._

@native.link("Typedef")
@native.extern
object Typedef {
	type enum_days = native.CInt
	type enum_toggle_e = native.CInt
	type toggle_e = enum_toggle_e
	type int2int = native.CFunctionPtr1[native.CInt, native.CInt]
	type day2string = native.CFunctionPtr1[enum_days, native.CString]
	type toggle = native.CFunctionPtr1[toggle_e, Unit]
}

import Typedef._

object TypedefEnums {
	final val enum_days_MONDAY = 0
	final val enum_days_TUESDAY = 1
	final val enum_days_WEDNESDAY = 2
	final val enum_days_THURSDAY = 3
	final val enum_days_FRIDAY = 4
	final val enum_days_SATURDAY = 5
	final val enum_days_SUNDAY = 6
	final val enum_toggle_e_OFF = 0
	final val enum_toggle_e_ON = 1
}
