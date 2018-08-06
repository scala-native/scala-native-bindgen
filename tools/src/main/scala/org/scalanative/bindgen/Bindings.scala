package org.scalanative.bindgen

import java.io.{File, PrintWriter}
import scala.collection.immutable.Seq

class Bindings(val name: String, val source: String, val errors: Seq[String]) {
  def writeToFile(file: File): Unit = {
    file.getParentFile.mkdirs()
    new PrintWriter(file) {
      write(source)
      close()
    }
  }
}
