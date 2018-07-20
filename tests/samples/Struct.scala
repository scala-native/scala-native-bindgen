package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Struct {
  type struct_point = native.CStruct2[native.CInt, native.CInt]
  type point = struct_point
  type struct_points = native.CStruct2[struct_point, point]
  type enum_pointIndex = native.CUnsignedInt
  type point_s = native.Ptr[struct_point]
  type struct_bigStruct = native.CArray[Byte, native.Nat.Digit[native.Nat._1, native.Nat.Digit[native.Nat._1, native.Nat._2]]]
  type struct_structWithAnonymousStruct = native.CStruct2[native.CInt, native.CArray[Byte, native.Nat._8]]
  type struct_packedStruct = native.CStruct1[native.CChar]
  type struct_bitFieldStruct = native.CArray[Byte, native.Nat._2]
  type enum_struct_op = native.CUnsignedInt
  def setPoints(points: native.Ptr[struct_points], x1: native.CInt, y1: native.CInt, x2: native.CInt, y2: native.CInt): Unit = native.extern
  def getPoint(points: native.Ptr[struct_points], pointIndex: enum_pointIndex): native.CInt = native.extern
  def createPoint(): native.Ptr[struct_point] = native.extern
  def getBigStructSize(): native.CInt = native.extern
  def getCharFromAnonymousStruct(s: native.Ptr[struct_structWithAnonymousStruct]): native.CChar = native.extern
  def getIntFromAnonymousStruct(s: native.Ptr[struct_structWithAnonymousStruct]): native.CChar = native.extern
  def struct_test_long(s: native.Ptr[struct_bigStruct], op: enum_struct_op, value: native.CLong): native.CInt = native.extern
  def struct_test_double(s: native.Ptr[struct_bigStruct], op: enum_struct_op, value: native.CDouble): native.CInt = native.extern
  def struct_test_point(s: native.Ptr[struct_bigStruct], op: enum_struct_op, value: native.Ptr[struct_point]): native.CInt = native.extern
}

import Struct._

object StructEnums {
  final val enum_pointIndex_X1: enum_pointIndex = 0.toUInt
  final val enum_pointIndex_Y1: enum_pointIndex = 1.toUInt
  final val enum_pointIndex_X2: enum_pointIndex = 2.toUInt
  final val enum_pointIndex_Y2: enum_pointIndex = 3.toUInt

  final val enum_struct_op_STRUCT_SET: enum_struct_op = 0.toUInt
  final val enum_struct_op_STRUCT_TEST: enum_struct_op = 1.toUInt
}

object StructHelpers {

  implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
    def x: native.CInt = !p._1
    def x_=(value: native.CInt): Unit = !p._1 = value
    def y: native.CInt = !p._2
    def y_=(value: native.CInt): Unit = !p._2 = value
  }

  def struct_point()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]

  implicit class struct_points_ops(val p: native.Ptr[struct_points]) extends AnyVal {
    def p1: native.Ptr[struct_point] = p._1
    def p1_=(value: native.Ptr[struct_point]): Unit = !p._1 = !value
    def p2: native.Ptr[point] = p._2
    def p2_=(value: native.Ptr[point]): Unit = !p._2 = !value
  }

  def struct_points()(implicit z: native.Zone): native.Ptr[struct_points] = native.alloc[struct_points]

  implicit class struct_bigStruct_ops(val p: native.Ptr[struct_bigStruct]) extends AnyVal {
    def one: native.CLong = !p._1.cast[native.Ptr[native.CLong]]
    def one_=(value: native.CLong): Unit = !p._1.cast[native.Ptr[native.CLong]] = value
    def two: native.CChar = !(p._1 + 8).cast[native.Ptr[native.CChar]]
    def two_=(value: native.CChar): Unit = !(p._1 + 8).cast[native.Ptr[native.CChar]] = value
    def three: native.CInt = !(p._1 + 12).cast[native.Ptr[native.CInt]]
    def three_=(value: native.CInt): Unit = !(p._1 + 12).cast[native.Ptr[native.CInt]] = value
    def four: native.CFloat = !(p._1 + 16).cast[native.Ptr[native.CFloat]]
    def four_=(value: native.CFloat): Unit = !(p._1 + 16).cast[native.Ptr[native.CFloat]] = value
    def five: native.CDouble = !(p._1 + 24).cast[native.Ptr[native.CDouble]]
    def five_=(value: native.CDouble): Unit = !(p._1 + 24).cast[native.Ptr[native.CDouble]] = value
    def six: native.Ptr[struct_point] = (p._1 + 32).cast[native.Ptr[struct_point]]
    def six_=(value: native.Ptr[struct_point]): Unit = !(p._1 + 32).cast[native.Ptr[struct_point]] = !value
    def seven: native.Ptr[struct_point] = !(p._1 + 40).cast[native.Ptr[native.Ptr[struct_point]]]
    def seven_=(value: native.Ptr[struct_point]): Unit = !(p._1 + 40).cast[native.Ptr[native.Ptr[struct_point]]] = value
    def eight: native.CInt = !(p._1 + 48).cast[native.Ptr[native.CInt]]
    def eight_=(value: native.CInt): Unit = !(p._1 + 48).cast[native.Ptr[native.CInt]] = value
    def nine: native.CInt = !(p._1 + 52).cast[native.Ptr[native.CInt]]
    def nine_=(value: native.CInt): Unit = !(p._1 + 52).cast[native.Ptr[native.CInt]] = value
    def ten: native.CInt = !(p._1 + 56).cast[native.Ptr[native.CInt]]
    def ten_=(value: native.CInt): Unit = !(p._1 + 56).cast[native.Ptr[native.CInt]] = value
    def eleven: native.CInt = !(p._1 + 60).cast[native.Ptr[native.CInt]]
    def eleven_=(value: native.CInt): Unit = !(p._1 + 60).cast[native.Ptr[native.CInt]] = value
    def twelve: native.CInt = !(p._1 + 64).cast[native.Ptr[native.CInt]]
    def twelve_=(value: native.CInt): Unit = !(p._1 + 64).cast[native.Ptr[native.CInt]] = value
    def thirteen: native.CInt = !(p._1 + 68).cast[native.Ptr[native.CInt]]
    def thirteen_=(value: native.CInt): Unit = !(p._1 + 68).cast[native.Ptr[native.CInt]] = value
    def fourteen: native.CInt = !(p._1 + 72).cast[native.Ptr[native.CInt]]
    def fourteen_=(value: native.CInt): Unit = !(p._1 + 72).cast[native.Ptr[native.CInt]] = value
    def fifteen: native.CInt = !(p._1 + 76).cast[native.Ptr[native.CInt]]
    def fifteen_=(value: native.CInt): Unit = !(p._1 + 76).cast[native.Ptr[native.CInt]] = value
    def sixteen: native.CInt = !(p._1 + 80).cast[native.Ptr[native.CInt]]
    def sixteen_=(value: native.CInt): Unit = !(p._1 + 80).cast[native.Ptr[native.CInt]] = value
    def seventeen: native.CInt = !(p._1 + 84).cast[native.Ptr[native.CInt]]
    def seventeen_=(value: native.CInt): Unit = !(p._1 + 84).cast[native.Ptr[native.CInt]] = value
    def eighteen: native.CInt = !(p._1 + 88).cast[native.Ptr[native.CInt]]
    def eighteen_=(value: native.CInt): Unit = !(p._1 + 88).cast[native.Ptr[native.CInt]] = value
    def nineteen: native.CInt = !(p._1 + 92).cast[native.Ptr[native.CInt]]
    def nineteen_=(value: native.CInt): Unit = !(p._1 + 92).cast[native.Ptr[native.CInt]] = value
    def twenty: native.CInt = !(p._1 + 96).cast[native.Ptr[native.CInt]]
    def twenty_=(value: native.CInt): Unit = !(p._1 + 96).cast[native.Ptr[native.CInt]] = value
    def twentyOne: native.CInt = !(p._1 + 100).cast[native.Ptr[native.CInt]]
    def twentyOne_=(value: native.CInt): Unit = !(p._1 + 100).cast[native.Ptr[native.CInt]] = value
    def twentyTwo: native.CInt = !(p._1 + 104).cast[native.Ptr[native.CInt]]
    def twentyTwo_=(value: native.CInt): Unit = !(p._1 + 104).cast[native.Ptr[native.CInt]] = value
    def twentyThree: native.CInt = !(p._1 + 108).cast[native.Ptr[native.CInt]]
    def twentyThree_=(value: native.CInt): Unit = !(p._1 + 108).cast[native.Ptr[native.CInt]] = value
  }

  def struct_bigStruct()(implicit z: native.Zone): native.Ptr[struct_bigStruct] = native.alloc[struct_bigStruct]

  implicit class struct_structWithAnonymousStruct_ops(val p: native.Ptr[struct_structWithAnonymousStruct]) extends AnyVal {
    def a: native.CInt = !p._1
    def a_=(value: native.CInt): Unit = !p._1 = value
    def anonymousStruct: native.Ptr[native.CArray[Byte, native.Nat._8]] = p._2
    def anonymousStruct_=(value: native.Ptr[native.CArray[Byte, native.Nat._8]]): Unit = !p._2 = !value
  }

  def struct_structWithAnonymousStruct()(implicit z: native.Zone): native.Ptr[struct_structWithAnonymousStruct] = native.alloc[struct_structWithAnonymousStruct]
}
