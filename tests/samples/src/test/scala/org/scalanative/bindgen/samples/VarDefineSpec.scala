package org.scalanative.bindgen.samples

import org.scalatest.FunSpec
import scala.scalanative.native._

class VarDefineSpec extends FunSpec {
  describe("variable #define bindings") {
    it("should have the value of the referenced variable") {
      assert(VarDefine.A == 23)
      assert(VarDefine.CONST_INT == 10)
    }
  }
}
