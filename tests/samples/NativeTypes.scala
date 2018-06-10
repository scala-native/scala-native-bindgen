import scala.scalanative._
import scala.scalanative.native._

@native.link("NativeTypes")
@native.extern
object NativeTypes {
  type size_t = native.CUnsignedInt
  type ptrdiff_t = native.CUnsignedInt
  type char16_t = native.CUnsignedShort
  type char32_t = native.CUnsignedInt
  type void_type = Unit
  type char_type = native.CChar
  type signed_char_type = native.CSignedChar
  type unsigned_char_type = native.CUnsignedChar
  type short_type = native.CShort
  type unsigned_short_type = native.CUnsignedShort
  type int_type = native.CInt
  type unsigned_int_type = native.CUnsignedInt
  type long_type = native.CLong
  type long_int_type = native.CLong
  type unsigned_long_type = native.CUnsignedLong
  type unsigned_long_int_type = native.CUnsignedLong
  type long_long_type = native.CLongLong
  type unsigned_long_long_type = native.CUnsignedLongLong
  type float_type = native.CFloat
  type double_type = native.CDouble
  type ptr_byte_type = native.Ptr[Byte]
  type ptr_int_type = native.Ptr[native.CInt]
  type cstring_type = native.CString
  type size_t_type = native.CSize
  type ptrdiff_t_type = native.CPtrDiff
  type char16_t_type = native.CChar16
  type char32_t_type = native.CChar32
}
