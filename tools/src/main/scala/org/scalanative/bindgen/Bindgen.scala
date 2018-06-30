package org.scalanative.bindgen

import java.io.File

import scala.collection.immutable
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
  def extraArg(args: String*): Bindgen

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArgBefore(args: String*): Bindgen

  /**
   * Run binding generator
   */
  def generate(): Bindings
}

object Bindgen {
  def apply(): Bindgen = Impl()

  private final case class Impl(
      executable: File = null,
      library: String = null,
      header: File = null,
      scalaObjectName: String = null,
      packageName: String = null,
      excludePrefix: String = null,
      extraArg: immutable.Seq[String] = immutable.Seq[String](),
      extraArgBefore: immutable.Seq[String] = immutable.Seq[String]())
      extends Bindgen {

    def bindgenExecutable(executable: File): Bindgen = {
      require(executable.exists())
      copy(executable = executable)
    }

    def header(header: File): Bindgen = {
      require(header.exists())
      copy(header = header)
    }

    def link(library: String): Bindgen = {
      require(!library.isEmpty)
      copy(library = library)
    }

    def scalaObjectName(scalaObjectName: String): Bindgen = {
      require(!scalaObjectName.isEmpty)
      copy(scalaObjectName = scalaObjectName)
    }

    def packageName(packageName: String): Bindgen = {
      require(!packageName.isEmpty)
      copy(packageName = packageName)
    }

    def excludePrefix(prefix: String): Bindgen = {
      require(!prefix.isEmpty)
      copy(excludePrefix = prefix)
    }

    def extraArg(args: String*): Bindgen = {
      require(args.forall(_.nonEmpty))
      copy(extraArg = extraArg ++ args)
    }

    def extraArgBefore(args: String*): Bindgen = {
      require(args.forall(_.nonEmpty))
      copy(extraArgBefore = extraArgBefore ++ args)
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

      val scalaObjectName =
        if (this.scalaObjectName != null)
          this.scalaObjectName
        else
          library

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
        cmd ++= Seq("--exclude-prefix", excludePrefix)
      }

      extraArg.foreach(arg => cmd ++= Seq("--extra-arg", arg))
      extraArgBefore.foreach(arg => cmd ++= Seq("--extra-arg-before", arg))

      cmd :+= "--"

      val output = Process(cmd).lineStream.mkString("\n")

      new Bindings(output)
    }
  }
}
