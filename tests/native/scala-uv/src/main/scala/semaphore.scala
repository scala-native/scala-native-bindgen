import scala.scalanative._
import scala.scalanative.native.Nat._
import types._

@native.extern
object semaphore {
  type union_sem_t = native.CArray[Byte, Digit[_2, Digit[_5, _6]]]
  type sem_t = union_sem_t
  def sem_init(__sem: native.Ptr[sem_t], __pshared: native.CInt, __value: native.CUnsignedInt): native.CInt = native.extern
  def sem_destroy(__sem: native.Ptr[sem_t]): native.CInt = native.extern
  def sem_open(__name: native.Ptr[native.CChar], __oflag: native.CInt): native.Ptr[sem_t] = native.extern
  def sem_close(__sem: native.Ptr[sem_t]): native.CInt = native.extern
  def sem_unlink(__name: native.Ptr[native.CChar]): native.CInt = native.extern
  def sem_wait(__sem: native.Ptr[sem_t]): native.CInt = native.extern
  def sem_timedwait(__sem: native.Ptr[sem_t], __abstime: native.Ptr[struct_timespec]): native.CInt = native.extern
  def sem_trywait(__sem: native.Ptr[sem_t]): native.CInt = native.extern
  def sem_post(__sem: native.Ptr[sem_t]): native.CInt = native.extern
  def sem_getvalue(__sem: native.Ptr[sem_t], __sval: native.Ptr[native.CInt]): native.CInt = native.extern
}
