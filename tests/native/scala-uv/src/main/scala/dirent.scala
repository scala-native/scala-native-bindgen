import scala.scalanative._
import scala.scalanative.native.Nat._

import types._

@native.link("netdb")
@native.extern
object dirent {
  type struct_dirent = native.CStruct5[__ino_t,__off_t,native.CUnsignedShort,native.CUnsignedChar,native.CArray[native.CChar, Digit[_2, Digit[_5, _6]]]]
  type DIR = native.CArray[Byte, Digit[_3, Digit[_2, _0]]]
  def opendir(__name: native.Ptr[native.CChar]): native.Ptr[DIR] = native.extern
  def fdopendir(__fd: native.CInt): native.Ptr[DIR] = native.extern
  def closedir(__dirp: native.Ptr[DIR]): native.CInt = native.extern
  def readdir(__dirp: native.Ptr[DIR]): native.Ptr[struct_dirent] = native.extern
  def readdir_r(__dirp: native.Ptr[DIR], __entry: native.Ptr[struct_dirent], __result: native.Ptr[native.Ptr[struct_dirent]]): native.CInt = native.extern
  def rewinddir(__dirp: native.Ptr[DIR]): Unit = native.extern
  def seekdir(__dirp: native.Ptr[DIR], __pos: native.CLong): Unit = native.extern
  def telldir(__dirp: native.Ptr[DIR]): native.CLong = native.extern
  def dirfd(__dirp: native.Ptr[DIR]): native.CInt = native.extern
  type size_t = native.CUnsignedLong
  def scandir(__dir: native.Ptr[native.CChar], __namelist: native.Ptr[native.Ptr[native.Ptr[struct_dirent]]], __selector: native.CFunctionPtr1[native.Ptr[struct_dirent],native.CInt], __cmp: native.CFunctionPtr2[native.Ptr[native.Ptr[struct_dirent]],native.Ptr[native.Ptr[struct_dirent]],native.CInt]): native.CInt = native.extern
  def alphasort(__e1: native.Ptr[native.Ptr[struct_dirent]], __e2: native.Ptr[native.Ptr[struct_dirent]]): native.CInt = native.extern
  def getdirentries(__fd: native.CInt, __buf: native.Ptr[native.CChar], __nbytes: native.CSize, __basep: native.Ptr[__off_t]): __ssize_t = native.extern
}

object direntEnums {
  final val enum_DT_UNKNOWN = 0
  final val enum_DT_FIFO = 1
  final val enum_DT_CHR = 2
  final val enum_DT_DIR = 3
  final val enum_DT_BLK = 4
  final val enum_DT_REG = 5
  final val enum_DT_LNK = 6
  final val enum_DT_SOCK = 7
  final val enum_DT_WHT = 8
}
