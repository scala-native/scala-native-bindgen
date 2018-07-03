package org.scalanative.bindgen

import java.io.File

import scala.collection.immutable.Seq
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
      extraArg: Seq[String] = Seq.empty,
      extraArgBefore: Seq[String] = Seq.empty)
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

    def generate(): Bindings = {
      require(executable.isDefined, "The executable must be specified")
      require(header.isDefined, "Header file must be specified")
      require(library.isDefined, "Library name must be specified")

      def withArgs(arg: String, values: Iterable[String]) =
        values.toSeq.flatMap(Seq(arg, _))

      var cmd = Seq(
        executable.get.getAbsolutePath,
        "--name",
        name.getOrElse(library.get),
        "--link",
        library.get
      ) ++
        withArgs("--package", packageName) ++
        withArgs("--exclude-prefix", excludePrefix) ++
        withArgs("--extra-arg", extraArg) ++
        withArgs("--extra-arg-before", extraArgBefore) ++
        Seq(header.get.getAbsolutePath, "--")

      val output = Process(cmd).lineStream.mkString("\n")

      new Bindings(output)
    }
  }
}
