package org.scalanative.bindgen

import java.io.{File, PrintWriter}

class Bindings(private val bindings: String, val errs: String) {
  def writeToFile(file: File): Unit = {
    file.getParentFile.mkdirs()
    new PrintWriter(file) {
      write(bindings)
      close()
    }
  }
}
