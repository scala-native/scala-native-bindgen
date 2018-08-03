package org.scalanative.bindgen

import java.io.File

import scala.collection.immutable.Seq
import scala.collection.mutable.ListBuffer
import scala.sys.process.{Process, ProcessLogger}

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
   * @return errors if exit code was not 0, otherwise return bindings
   */
  def generate(): Either[Seq[String], Bindings]
}

object Bindgen {
  def apply(): Bindgen = Impl()

  private final case class Impl(executable: Option[File] = None,
                                library: Option[String] = None,
                                header: Option[File] = None,
                                name: Option[String] = None,
                                packageName: Option[String] = None,
                                excludePrefix: Option[String] = None,
                                extraArg: Seq[String] = Seq.empty,
                                extraArgBefore: Seq[String] = Seq.empty)
      extends Bindgen {

    def bindgenExecutable(executable: File): Bindgen = {
      require(executable.exists(), s"Executable does not exist: $executable")
      copy(executable = Option(executable))
    }

    def header(header: File): Bindgen = {
      require(header.exists(), s"Header file does not exist: $header")
      copy(header = Option(header))
    }

    def link(library: String): Bindgen = {
      require(library.nonEmpty, "Library must be non-empty")
      copy(library = Option(library))
    }

    def name(name: String): Bindgen = {
      require(name.nonEmpty, "Name must be non-empty")
      copy(name = Option(name))
    }

    def packageName(packageName: String): Bindgen = {
      require(packageName.nonEmpty, "Package name must be non-empty")
      copy(packageName = Option(packageName))
    }

    def excludePrefix(prefix: String): Bindgen = {
      require(prefix.nonEmpty, "Exclude prefix must be non-empty")
      copy(excludePrefix = Option(prefix))
    }

    def extraArg(args: String*): Bindgen = {
      require(args.forall(_.nonEmpty), "All extra-args must be non-empty")
      copy(extraArg = extraArg ++ args)
    }

    def extraArgBefore(args: String*): Bindgen = {
      require(args.forall(_.nonEmpty),
              "All extra-args-before must be non-empty")
      copy(extraArgBefore = extraArgBefore ++ args)
    }

    def generate(): Either[Seq[String], Bindings] = {
      require(executable.isDefined, "The executable must be specified")
      require(header.isDefined, "Header file must be specified")

      val nameOrLibrary = name.orElse(library)
      require(nameOrLibrary.isDefined,
              "Name must be specified when no library name is given")

      def withArgs(arg: String, values: Iterable[String]) =
        values.toSeq.flatMap(Seq(arg, _))

      val cmd = Seq(executable.get.getAbsolutePath) ++
        withArgs("--name", nameOrLibrary) ++
        withArgs("--link", library) ++
        library.fold(Seq("--no-link"))(_ => Seq.empty) ++
        withArgs("--package", packageName) ++
        withArgs("--exclude-prefix", excludePrefix) ++
        withArgs("--extra-arg", extraArg) ++
        withArgs("--extra-arg-before", extraArgBefore) ++
        Seq(header.get.getAbsolutePath, "--")

      val stdout = ListBuffer[String]()
      val stderr = ListBuffer[String]()
      val nl     = System.lineSeparator()
      val logger = ProcessLogger(stdout.+=, stderr.+=)

      Process(cmd).!(logger) match {
        case 0 => Right(new Bindings(stdout.mkString(nl), Seq(stderr: _*)))
        case _ => Left(Seq(stderr: _*))
      }
    }
  }
}
