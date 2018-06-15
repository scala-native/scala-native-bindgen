package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.UnionHelpers._

object UnionTests extends TestSuite {
  val tests = Tests {
    'getValues - {
      val point = Union.getValues()
      assert(point.a == 10)
    }
  }
}
