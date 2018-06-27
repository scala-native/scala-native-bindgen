package org.scalanative.bindgen

import scala.collection.mutable

class Builder {
  private var library: String                     = _
  private var pathToHeader: String                = _
  private var packageName: String                 = _
  private var excludePrefix: String               = _
  private var extraArg: mutable.Seq[String]       = mutable.Seq()
  private var extraArgBefore: mutable.Seq[String] = mutable.Seq()

  /**
   * Set header file for which bindings will be generated
   */
  def header(pathToHeader: String): Unit = {
    this.pathToHeader = pathToHeader
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
    if (!prefix.isEmpty) {
      excludePrefix = prefix
    }
  }

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArg(arg: String): Unit = {
    if (!arg.isEmpty) {
      extraArg += arg
    }
  }

  /**
   * Additional argument to append to the compiler command line
   */
  def extraArgBefore(arg: String): Unit = {
    if (!arg.isEmpty) {
      extraArgBefore += arg
    }
  }

  /**
   * Run binding generator
   */
  def generate(): Bindings = {
    // TODO: generate bindings
    new Bindings("")
  }
}

object Builder {
  def apply(): Builder = new Builder()
}
