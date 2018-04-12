import scala.scalanative._

@native.extern
@native.link("scbintest")
object scbintest {
  type struct_Point = native.CStruct2[native.CInt,native.CInt]
  type Point = struct_Point
  type struct_foo = native.CStruct2[native.CFloat,native.CChar]
  type foo = struct_foo
  def sumi(a: native.CInt, b: native.CInt): native.CInt = native.extern
  def sumd(a: native.CDouble, b: native.CDouble): native.CDouble = native.extern
  def doublePoint(p: native.Ptr[Point]): Unit = native.extern
}


object Hello extends App {
  println("test: " + scbintest.sumi(1,2))


  val p = native.stackalloc[scbintest.Point]
  scbintest.doublePoint(p)
  println("test: " + p._1)
}
