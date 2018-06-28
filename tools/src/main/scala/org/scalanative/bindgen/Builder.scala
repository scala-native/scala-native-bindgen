package org.scalanative.bindgen

import java.io.File

import scala.collection.mutable

sealed trait Builder {

  /**
   * Set header file for which bindings will be generated
   */
  def header(header: File): Unit

  /**
   * Library to link with, e.g. -luv for libuv
   */
  def link(library: String): Unit

  /**
   * Package name of generated Scala file
   */
  def packageName(packageName: String): Unit

  /**
   * Declarations will be removed if their names
   * contain given prefix
   */
  def excludePrefix(prefix: String): Unit

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArg(arg: String): Unit

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArgBefore(arg: String): Unit

  /**
   * Run binding generator
   */
  def generate(): Bindings
}

object Builder {
  def apply(): Builder = Impl()

  private final case class Impl() extends Builder {
    private var library: String                     = _
    private var header: File                        = _
    private var packageName: String                 = _
    private var excludePrefix: String               = _
    private var extraArg: mutable.Seq[String]       = mutable.Seq()
    private var extraArgBefore: mutable.Seq[String] = mutable.Seq()

    /**
     * Set header file for which bindings will be generated
     */
    def header(header: File): Unit = {
      this.header = header
    }

    /**
     * Library to link with, e.g. -luv for libuv
     */
    def link(library: String): Unit = {
      this.library = library
    }

    /**
     * Package name of generated Scala file
     */
    def packageName(packageName: String): Unit = {
      this.packageName = packageName
    }

    /**
     * Declarations will be removed if their names
     * contain given prefix
     */
    def excludePrefix(prefix: String): Unit = {
      require(!prefix.isEmpty)
      excludePrefix = prefix
    }

    /**
     * Additional argument to append to the compiler command line
     */
    def extraArg(arg: String): Unit = {
      require(!arg.isEmpty)
      extraArg :+ arg
    }

    /**
     * Additional argument to append to the compiler command line
     */
    def extraArgBefore(arg: String): Unit = {
      require(!arg.isEmpty)
      extraArgBefore :+ arg
    }

    /**
     * Run binding generator
     */
    def generate(): Bindings = {
      // TODO: generate bindings
      new Bindings("")
    }
  }
}
