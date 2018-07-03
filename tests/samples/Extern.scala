package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.extern
object Extern {
  type enum_mode = native.CUnsignedInt
  type struct_version = native.CStruct3[native.CInt, native.CInt, native.CInt]
  val forty_two: native.CInt = native.extern
  val version: native.CString = native.extern
  val mode: enum_mode = native.extern
  val semver: native.Ptr[struct_version] = native.extern
}

import Extern._

object ExternEnums {
  final val enum_mode_SYSTEM: enum_mode = 0.toUInt
  final val enum_mode_USER: enum_mode = 1.toUInt
}

object ExternHelpers {

  implicit class struct_version_ops(val p: native.Ptr[struct_version]) extends AnyVal {
    def major: native.CInt = !p._1
    def major_=(value: native.CInt): Unit = !p._1 = value
    def minor: native.CInt = !p._2
    def minor_=(value: native.CInt): Unit = !p._2 = value
    def patch: native.CInt = !p._3
    def patch_=(value: native.CInt): Unit = !p._3 = value
  }

  def struct_version()(implicit z: native.Zone): native.Ptr[struct_version] = native.alloc[struct_version]
}
