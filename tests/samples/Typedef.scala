package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

object Typedef {
  type enum_days = native.CUnsignedInt
  object enum_days {
    final val MONDAY: enum_days = 0.toUInt
    final val TUESDAY: enum_days = 1.toUInt
    final val WEDNESDAY: enum_days = 2.toUInt
    final val THURSDAY: enum_days = 3.toUInt
    final val FRIDAY: enum_days = 4.toUInt
    final val SATURDAY: enum_days = 5.toUInt
    final val SUNDAY: enum_days = 6.toUInt
  }

  type enum_toggle_e = native.CUnsignedInt
  object enum_toggle_e {
    final val OFF: enum_toggle_e = 0.toUInt
    final val ON: enum_toggle_e = 1.toUInt
  }

  type toggle_e = enum_toggle_e
  type int2int = native.CFunctionPtr1[native.CInt, native.CInt]
  type day2string = native.CFunctionPtr1[enum_days, native.CString]
  type toggle = native.CFunctionPtr1[toggle_e, Unit]
}
