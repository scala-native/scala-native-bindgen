import scala.scalanative._
import scala.scalanative.native._
import scala.scalanative.native.Nat._

@native.link("Struct")
@native.extern
object Struct {
	type struct_point = native.CStruct2[native.CInt, native.CInt]
	type point_s = native.Ptr[struct_point]
}

import Struct._

object StructHelpers {
	implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
		def x: native.CInt = !p._1
		def x_=(value: native.CInt):Unit = !p._1 = value
		def y: native.CInt = !p._2
		def y_=(value: native.CInt):Unit = !p._2 = value
	}

	def struct_point()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]

}
