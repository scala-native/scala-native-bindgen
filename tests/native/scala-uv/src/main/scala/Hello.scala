import scala.scalanative._

object Hello extends App {
  val time = native.stackalloc[native.CDouble]
  val result = uv.uv_uptime(time)
  //println("time: " + time + " result: " + result)
  println("Return " + result + " uptime: " + !time)
}