import org.scalanative.bindgen.BindingOptions

object BindingHelpers {
  implicit class BindingOptionsOps(val options: BindingOptions) extends AnyVal {
    type F[T] = BindingOptions => T => BindingOptions

    def maybe[T](opt: Option[T], f: F[T]): BindingOptions =
      opt match {
        case None        => options
        case Some(value) => f(options)(value)
      }
  }
}
