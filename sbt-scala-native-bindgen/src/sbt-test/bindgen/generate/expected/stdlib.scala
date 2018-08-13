package org.example.app.stdlib

import scala.scalanative._
import scala.scalanative.native._

@native.extern
object stdlib {
  def access(path: native.CString, mode: native.CInt): native.CInt = native.extern
  def read(fildes: native.CInt, buf: native.Ptr[Byte], nbyte: native.CInt): native.CInt = native.extern
  def printf(format: native.CString, varArgs: native.CVararg*): native.CInt = native.extern
}
