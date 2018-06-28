package org.scalanative.bindgen

import java.io.File

import scala.collection.mutable
import scala.sys.process.Process

sealed trait Bindgen {

  /**
   * Set bindgen executable
   */
  def bindgenExecutable(executable: File): Bindgen

  /**
   * Set header file for which bindings will be generated
   */
  def header(header: File): Bindgen

  /**
   * Library to link with, e.g. -luv for libuv
   */
  def link(library: String): Bindgen

  /**
   * Name of Scala object that contains bindings.
   * Default is set to library name.
   */
  def scalaObjectName(scalaObjectName: String): Bindgen

  /**
   * Package name of generated Scala file
   */
  def packageName(packageName: String): Bindgen

  /**
   * Declarations will be removed if their names
   * contain given prefix
   */
  def excludePrefix(prefix: String): Bindgen

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArg(arg: String): Bindgen

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArgBefore(arg: String): Bindgen

  /**
   * Run binding generator
   */
  def generate(): Bindings
}

object Bindgen {
  def apply(): Bindgen = Impl()

  private final case class Impl() extends Bindgen {
    private var executable: File                    = _
    private var library: String                     = _
    private var header: File                        = _
    private var scalaObjectName: String             = _
    private var packageName: String                 = _
    private var excludePrefix: String               = _
    private val extraArg: mutable.Seq[String]       = mutable.Seq()
    private val extraArgBefore: mutable.Seq[String] = mutable.Seq()

    def bindgenExecutable(executable: File): Bindgen = {
      require(executable.exists())
      this.executable = executable
      this
    }

    def header(header: File): Bindgen = {
      require(header.exists())
      this.header = header
      this
    }

    def link(library: String): Bindgen = {
      require(!library.isEmpty)
      this.library = library
      this
    }

    def scalaObjectName(scalaObjectName: String): Bindgen = {
      require(!scalaObjectName.isEmpty)
      this.scalaObjectName = scalaObjectName
      this
    }

    def packageName(packageName: String): Bindgen = {
      require(!packageName.isEmpty)
      this.packageName = packageName
      this
    }

    def excludePrefix(prefix: String): Bindgen = {
      require(!prefix.isEmpty)
      excludePrefix = prefix
      this
    }

    def extraArg(arg: String): Bindgen = {
      require(!arg.isEmpty)
      extraArg :+ arg
      this
    }

    def extraArgBefore(arg: String): Bindgen = {
      require(!arg.isEmpty)
      extraArgBefore :+ arg
      this
    }

    private def validateFields(): Unit = {
      if (executable == null) {
        throw new AssertionError("Specify bindgen executable")
      }
      if (header == null) {
        throw new AssertionError("Specify header file")
      }
      if (library == null) {
        throw new AssertionError("Specify library name")
      }
    }

    def generate(): Bindings = {
      validateFields()

      if (scalaObjectName == null) {
        scalaObjectName = library
      }
      var cmd = Seq(
        executable.getAbsolutePath,
        header.getAbsolutePath,
        "--name",
        scalaObjectName,
        "--link",
        library
      )

      if (packageName != null) {
        cmd ++= Seq("--package", packageName)
      }

      if (excludePrefix != null) {
        cmd :+= excludePrefix
      }

      cmd :+= "--"

      // TODO: extra args

      val output = Process(cmd).lineStream.mkString("\n")

      new Bindings(output)
    }
  }
}
