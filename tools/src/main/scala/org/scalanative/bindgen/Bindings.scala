package org.scalanative.bindgen

import java.io.{File, PrintWriter}

class Bindings(private val bindings: String) {
  def writeToFile(file: File): Unit = {
    new PrintWriter(file) {
      write(bindings)
      close()
    }
  }
}
