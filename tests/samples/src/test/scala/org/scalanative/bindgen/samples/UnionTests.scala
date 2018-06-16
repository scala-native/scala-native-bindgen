package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.UnionHelpers._

object UnionTests extends TestSuite {
  val tests = Tests {
    'getValues - {
      Zone {implicit zone =>
        val structPtr = alloc[Union.union_values]
        Union.setIntValue(structPtr)
        assert(!structPtr.i == 10)
        Union.setLongValue(structPtr)
        assert(!structPtr.l == 10000000000L)
      }
    }
  }
}
