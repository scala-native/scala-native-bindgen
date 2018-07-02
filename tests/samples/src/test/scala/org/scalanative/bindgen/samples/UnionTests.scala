package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.UnionHelpers._

object UnionTests extends TestSuite {
  val tests = Tests {
    'getValues - {
      Zone {implicit zone =>
        val unionPtr = alloc[Union.union_values]
        Union.setIntValue(unionPtr)
        assert(!unionPtr.i == 10)
        Union.setLongValue(unionPtr)
        assert(!unionPtr.l == 10000000000L)
      }
    }

    'unionSize - {
      assert(Union.getUnionSize() == sizeof[Union.union_values])
    }
  }
}
