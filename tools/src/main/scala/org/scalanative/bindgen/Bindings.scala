package org.scalanative.bindgen

import java.io.{File, PrintWriter}
import scala.collection.immutable.Seq

class Bindings(private val bindings: String, val errors: Seq[String]) {
  def writeToFile(file: File): Unit = {
    file.getParentFile.mkdirs()
    new PrintWriter(file) {
      write(bindings)
      close()
    }
  }
}
