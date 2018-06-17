package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

object DefineDefines {
  val STRING: native.CString = c"Hello, World!"
  val INT = 42
  val FLOAT = 5.6
}
