package org.scalanative.bindgen.samples

import utest._
import scalanative.native._

object EnumTests extends TestSuite {
  val tests = Tests {
    'get_WEDNESDAY - {
      assert(Enum.get_WEDNESDAY() == EnumEnums.enum_days_WEDNESDAY)
    }

    'check_BIG_NEG_A - {
      assert(Enum.check_BIG_NEG_A(EnumEnums.enum_bigNegativeValues_BIG_NEG_A) == c"OK")
    }
  }
}
