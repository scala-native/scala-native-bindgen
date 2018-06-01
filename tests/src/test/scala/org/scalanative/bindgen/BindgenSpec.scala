package org.scalanative.bindgen

import java.io.{File, PrintWriter}
import org.scalatest.FunSpec
import scala.io.Source
import scala.sys.process._

class BindgenSpec extends FunSpec {
  describe("Bindgen") {
    val bindgenPath    = System.getProperty("bindgen.path")
    val inputDirectory = new File("samples")

    val outputDir = new File("target/bindgen-samples")
    Option(outputDir.listFiles()).foreach(_.foreach(_.delete()))
    outputDir.mkdirs()

    it("should exist") {
      assert(new File(bindgenPath).exists)
    }

    def bindgen(inputFile: File, name: String, outputFile: File): Unit = {
      val cmd = Seq(
        bindgenPath,
        inputFile.getAbsolutePath,
        "-name",
        name,
        "--"
      )
      val output = Process(cmd).lineStream.mkString("\n")

      new PrintWriter(outputFile) {
        write(output)
        close()
      }
    }

    def contentOf(file: File) =
      Source.fromFile(file).getLines.mkString("\n").trim()

    for (input <- inputDirectory.listFiles() if input.getName.endsWith(".h")) {
      it(s"should generate bindings for ${input.getName}") {
        val testName = input.getName.replace(".h", "")
        val expected = new File(inputDirectory, testName + ".scala")
        val output   = new File(outputDir, testName + ".scala")

        bindgen(input, testName, output)

        assert(output.exists())
        assert(contentOf(output) == contentOf(expected))
      }
    }
  }
}
