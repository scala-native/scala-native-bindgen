package org.scalanative.bindgen.samples

import utest._
import org.scalanative.bindgen.samples.StructHelpers._

object StructTests extends TestSuite {
  val tests = Tests {
    'getPoint - {
      val point = Struct.getPoint()
      assert(point.x == 10)
      assert(point.y == 20)
    }
  }
}
