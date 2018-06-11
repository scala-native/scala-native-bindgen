import scala.scalanative._
import scala.scalanative.native._

@native.link("native")
@native.extern
object nativeLib {
  type nativeFunc0 = native.CInt
  @native.link("native")
  def nativeFunc0(anonymous0: native.CInt): Unit = native.extern
  def nativeFunc(anonymous0: native.CFloat): Unit = native.extern
}
