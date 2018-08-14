package org.example.app.core

import scala.scalanative._
import scala.scalanative.native._

@native.link("core")
@native.extern
object core {
  def count_words(text: native.CString): native.CInt = native.extern
  def __not_excluded(): Unit = native.extern
}
