package org.example.wordcount

import scala.scalanative._
import scala.scalanative.native._

@native.link("wordcount")
@native.extern
object WordCount {
  type struct_wordcount = native.CStruct3[native.CLong, native.CLong, native.CLong]
  //#using-stdio-file
  def wordcount(wordcount: native.Ptr[struct_wordcount], file: native.Ptr[scala.scalanative.native.stdio.FILE]): native.CInt = native.extern
  //#using-stdio-file

  object implicits {
    implicit class struct_wordcount_ops(val p: native.Ptr[struct_wordcount]) extends AnyVal {
      def chars: native.CLong = !p._1
      def chars_=(value: native.CLong): Unit = !p._1 = value
      def lines: native.CLong = !p._2
      def lines_=(value: native.CLong): Unit = !p._2 = value
      def words: native.CLong = !p._3
      def words_=(value: native.CLong): Unit = !p._3 = value
    }
  }

  object struct_wordcount {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_wordcount] = native.alloc[struct_wordcount]
    def apply(chars: native.CLong, lines: native.CLong, words: native.CLong)(implicit z: native.Zone): native.Ptr[struct_wordcount] = {
      val ptr = native.alloc[struct_wordcount]
      ptr.chars = chars
      ptr.lines = lines
      ptr.words = words
      ptr
    }
  }
}
