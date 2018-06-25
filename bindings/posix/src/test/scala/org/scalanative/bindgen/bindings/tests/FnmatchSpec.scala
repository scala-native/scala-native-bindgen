package org.scalanative.bindgen.bindings.tests

import org.scalatest.FunSpec

class FnmatchSpec extends FunSpec {
  describe("fnmatch") {
    it("should match patterns") {
      //#usage-example
      import scala.scalanative.native._
      import org.scalanative.bindgen.bindings.posix.fnmatch._

      assert(fnmatch(c"*.md", c"README.md", 0) == 0)
      assert(fnmatch(c"*.[ch]", c"main.h", 0) == 0)
      assert(fnmatch(c"*.[ch]", c"main.c", 0) == 0)
      assert(fnmatch(c"*.md", c"README_md", 0) == defines.FNM_NOMATCH)
      //#usage-example
    }
  }
}
