package org.scalanative.bindgen

import java.io.{File, PrintWriter}

class Bindings(private val bindings: String) {
  def writeToFile(pathToFile: String): Unit = {
    val pw = new PrintWriter(new File(pathToFile))
    try {
      pw.write(bindings)
    } finally {
      pw.close()
    }
  }
}
