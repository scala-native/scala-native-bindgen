import scala.scalanative._

@native.extern
object stdint {
  type int8_t = native.CSignedChar
  type int16_t = native.CShort
  type int32_t = native.CInt
  type int64_t = native.CLong
  type uint8_t = native.CUnsignedChar
  type uint16_t = native.CUnsignedShort
  type uint32_t = native.CUnsignedInt
  type uint64_t = native.CUnsignedLong
  type int_least8_t = native.CSignedChar
  type int_least16_t = native.CShort
  type int_least32_t = native.CInt
  type int_least64_t = native.CLong
  type uint_least8_t = native.CUnsignedChar
  type uint_least16_t = native.CUnsignedShort
  type uint_least32_t = native.CUnsignedInt
  type uint_least64_t = native.CUnsignedLong
  type int_fast8_t = native.CSignedChar
  type int_fast16_t = native.CLong
  type int_fast32_t = native.CLong
  type int_fast64_t = native.CLong
  type uint_fast8_t = native.CUnsignedChar
  type uint_fast16_t = native.CUnsignedLong
  type uint_fast32_t = native.CUnsignedLong
  type uint_fast64_t = native.CUnsignedLong
  type intptr_t = native.CLong
  type uintptr_t = native.CUnsignedLong
  type intmax_t = native.CLong
  type uintmax_t = native.CUnsignedLong
}
