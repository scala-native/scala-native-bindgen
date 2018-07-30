package org.scalanative.bindgen

import java.io.File

sealed trait BindingOptions {

  /**
   * Library to link with, e.g. -luv for libuv
   */
  def link(library: String): BindingOptions

  /**
   * Name of Scala object that contains bindings.
   * Default is set to library name.
   */
  def name(name: String): BindingOptions

  /**
   * Package name of generated Scala file
   */
  def packageName(packageName: String): BindingOptions

  /**
   * Declarations will be removed if their names
   * contain given prefix
   */
  def excludePrefix(prefix: String): BindingOptions

  /**
   * Additional arguments to append to the compiler command line
   */
  def extraArgs(args: String*): BindingOptions

  /**
   * Additional arguments to prepend to the compiler command line
   */
  def extraArgsBefore(args: String*): BindingOptions

  /**
   * Reuse types from already generated bindings.
   * @param config file that contains information about generated bindings.
   */
  def bindingConfig(config: File): BindingOptions

}

object BindingOptions {
  def apply(header: File): BindingOptions = {
    require(header.exists(), s"Header does not exist: `$header`")
    Impl(header = header)
  }

  private[bindgen] final case class Impl(header: File,
                                         library: Option[String] = None,
                                         name: Option[String] = None,
                                         packageName: Option[String] = None,
                                         excludePrefix: Option[String] = None,
                                         extraArgs: Seq[String] = Seq.empty,
                                         extraArgsBefore: Seq[String] =
                                           Seq.empty,
                                         bindingConfig: Option[File] = None)
      extends BindingOptions {

    override def link(library: String): BindingOptions = {
      require(!library.isEmpty, "Library name must be non-empty")
      copy(library = Option(library))
    }

    override def name(name: String): BindingOptions = {
      require(!name.isEmpty, "Scala object name must be non-empty")
      copy(name = Option(name))
    }

    override def packageName(packageName: String): BindingOptions = {
      require(!packageName.isEmpty, "Package name must be non-empty")
      copy(packageName = Option(packageName))
    }

    override def excludePrefix(prefix: String): BindingOptions = {
      require(!prefix.isEmpty, "Exclude prefix must be non-empty")
      copy(excludePrefix = Option(prefix))
    }

    override def extraArgs(args: String*): BindingOptions = {
      require(args.forall(_.nonEmpty), "All extra-args must be non-empty")
      copy(extraArgs = extraArgs ++ args)
    }

    override def extraArgsBefore(args: String*): BindingOptions = {
      require(args.forall(_.nonEmpty),
              "All extra-args-before must be non-empty")
      copy(extraArgsBefore = extraArgsBefore ++ args)
    }

    override def bindingConfig(config: File): BindingOptions = {
      require(config.exists(), s"Config file must exist: $config")
      copy(bindingConfig = Some(config))
    }

  }
}
