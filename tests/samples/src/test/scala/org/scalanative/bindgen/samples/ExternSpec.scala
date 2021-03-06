package org.scalanative.bindgen.samples

import org.scalatest.FunSpec
import scalanative.native._

class ExternSpec extends FunSpec {
  describe("extern variable bindings") {
    it("should bind to C variable") {
      assert(Extern.forty_two == 42)
      assert(Extern.mode == Extern.enum_mode.USER)
    }

    it("should bind to C struct variable") {
      import Extern.implicits._
      assert(
        Extern.semver.major == 1 && Extern.semver.minor == 2 && Extern.semver.patch == 3)
    }
  }
}
