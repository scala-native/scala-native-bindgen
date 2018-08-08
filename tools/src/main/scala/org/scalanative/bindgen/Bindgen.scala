package org.scalanative.bindgen

import java.io.File

import scala.collection.immutable.Seq
import scala.collection.mutable.ListBuffer
import scala.sys.process.{Process, ProcessLogger}

class Bindgen(val executable: File) {

  /**
   * Run binding generator using the given scala-native-bindgen executable.
   * @return errors if exit code was not 0, otherwise return bindings
   */
  def generate(options: BindingOptions): Either[Seq[String], Bindings] = {
    options match {
      case impl: BindingOptions.Impl =>
        import impl._

        require(executable.exists, "The executable does not exist")

        val nameOrLibrary = name.orElse(library)
        require(nameOrLibrary.isDefined,
                "Name must be specified when no library name is given")

        def withArgs(arg: String, values: Iterable[String]) =
          values.toSeq.flatMap(Seq(arg, _))

        val options =
          withArgs("--name", nameOrLibrary) ++
            withArgs("--link", library) ++
            library.fold(Seq("--no-link"))(_ => Seq.empty) ++
            withArgs("--package", packageName) ++
            withArgs("--exclude-prefix", excludePrefix) ++
            withArgs("--extra-arg", extraArgs) ++
            withArgs("--extra-arg-before", extraArgsBefore) ++
            withArgs("--binding-config", bindingConfig.map(_.getAbsolutePath)) ++
            Seq(header.getAbsolutePath, "--")

        val cmd    = Seq(executable.getAbsolutePath) ++ options
        val stdout = ListBuffer[String]()
        val stderr = ListBuffer[String]()
        val nl     = System.lineSeparator()
        val logger = ProcessLogger(stdout.+=, stderr.+=)

        Process(cmd).!(logger) match {
          case 0 =>
            Right(
              new Bindings(nameOrLibrary.get,
                           stdout.mkString(nl),
                           Seq(stderr: _*)))
          case _ => Left(Seq(stderr: _*))
        }

      case _ => Left(Seq("Illtyped BindingOptions"))
    }
  }

}

object Bindgen {
  def apply(executable: File) =
    new Bindgen(executable)
}
