import scala.scalanative._
import scala.scalanative.native._
import scala.scalanative.native.Nat._

@native.link("Function")
@native.extern
object Function {
	def no_args(): native.CInt = native.extern
	def void_arg(): native.CInt = native.extern
	def one_arg(a: native.CInt): Unit = native.extern
	def two_args(a: native.CFloat, b: native.CInt): native.Ptr[Byte] = native.extern
	def anonymous_args(anonymous0: native.CFloat, anonymous1: native.CInt): Unit = native.extern
	def variadic_args(a: native.CDouble, b: native.Ptr[Byte], varArgs: native.CVararg*): native.CDouble = native.extern
}
