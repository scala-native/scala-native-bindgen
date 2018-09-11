package org.scalanative.bindings.tests

import org.scalatest.FunSpec

class Utf8procSpec extends FunSpec {
  describe("utf8proc") {
    it("should iterate UTF-8 and count character width") {
      //#usage-example
      import org.scalanative.bindings.utf8proc._
      import scala.scalanative.native._

      val text    = c"Spørge"
      val textlen = string.strlen(text)

      val codepoint        = stackalloc[utf8proc_int32_t]
      var textpos: CSize   = 0
      var textwidth: CSize = 0

      while (textpos < textlen) {
        val bytes = utf8proc_iterate(
          text.cast[Ptr[UByte]] + textpos,
          textlen - textpos,
          codepoint
        )
        textwidth += utf8proc_charwidth(!codepoint)
        textpos += bytes
      }

      assert(textlen == 7)
      assert(textwidth == 6)
      //#usage-example
    }

    it("utf8proc_tolower") {
      import org.scalanative.bindings.utf8proc._

      val `Ø`  = 0x00D8
      val `ø`  = 0x00F8
      val `😱` = 0x1F631

      assert(utf8proc_tolower(`Ø`) == `ø`)
      assert(utf8proc_tolower(`😱`) == `😱`)
    }
  }
}
