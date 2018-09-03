package org.scalanative.bindgen.docs

import org.scalatest.FunSpec

class WordCountSpec extends FunSpec {
  describe("wordcount") {
    it("using generated bindings") {
      //#example
      import org.example.wordcount.WordCount._
      import scalanative.native._

      //#example
      val pathToFile =
        c"docs/src/test/resources/scala-native-bindings/wordcount.h"

      import scalanative.posix.unistd.access
      import scalanative.posix.fcntl.R_OK
      assert(access(pathToFile, R_OK) == 0, "Header file does not exist")

      //#example
      Zone { implicit zone =>
        val result = struct_wordcount()
        val file   = stdio.fopen(pathToFile, c"r")
        val code   = wordcount(result, file)
        stdio.fclose(file)
        //#example
        import org.example.wordcount.WordCount.implicits._
        assert(code == 0)
        assert(result.chars == 187)
        assert(result.words == 20)
        assert(result.lines == 11)
      //#example
      }
      //#example
    }
  }
}
