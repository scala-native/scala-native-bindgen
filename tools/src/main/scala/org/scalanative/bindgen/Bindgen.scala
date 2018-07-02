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
  def name(name: String): Bindgen

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
      executable: Option[File] = None,
      library: Option[String] = None,
      header: Option[File] = None,
      name: Option[String] = None,
      packageName: Option[String] = None,
      excludePrefix: Option[String] = None,
      extraArg: immutable.Seq[String] = immutable.Seq[String](),
      extraArgBefore: immutable.Seq[String] = immutable.Seq[String]())
      extends Bindgen {

    def bindgenExecutable(executable: File): Bindgen = {
      require(executable.exists())
      copy(executable = Option(executable))
    }

    def header(header: File): Bindgen = {
      require(header.exists())
      copy(header = Option(header))
    }

    def link(library: String): Bindgen = {
      require(!library.isEmpty)
      copy(library = Option(library))
    }

    def name(name: String): Bindgen = {
      require(!name.isEmpty)
      copy(name = Option(name))
    }

    def packageName(packageName: String): Bindgen = {
      require(!packageName.isEmpty)
      copy(packageName = Option(packageName))
    }

    def excludePrefix(prefix: String): Bindgen = {
      require(!prefix.isEmpty)
      copy(excludePrefix = Option(prefix))
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
      require(executable.isDefined, "The executable must be specified")
      require(header.isDefined, "Header file must be specified")
      require(library.isDefined, "Library name must be specified")
    }

    def generate(): Bindings = {
      validateFields()

      val name = this.name.getOrElse(library.get)

      var cmd = Seq(
        executable.get.getAbsolutePath,
        header.get.getAbsolutePath,
        "--name",
        name,
        "--link",
        library.get
      )

      if (packageName.isDefined) {
        cmd ++= Seq("--package", packageName.get)
      }

      if (excludePrefix.isDefined) {
        cmd ++= Seq("--exclude-prefix", excludePrefix.get)
      }

      extraArg.foreach(arg => cmd ++= Seq("--extra-arg", arg))
      extraArgBefore.foreach(arg => cmd ++= Seq("--extra-arg-before", arg))

      cmd :+= "--"

      val output = Process(cmd).lineStream.mkString("\n")

      new Bindings(output)
    }
  }
}
