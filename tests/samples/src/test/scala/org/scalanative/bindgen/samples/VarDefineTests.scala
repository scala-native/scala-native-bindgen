package org.scalanative.bindgen.samples

import utest._

object VarDefineTests extends TestSuite {
  val tests = Tests {
    'getA - {
      assert(VarDefine.getA() == 23)
    }
  }
}
