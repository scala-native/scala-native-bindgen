package org.scalanative.bindgen.samples

import scala.scalanative._
import scala.scalanative.native._

@native.link("bindgentests")
@native.extern
object Struct {
  type enum_pointIndex = native.CUnsignedInt
  object enum_pointIndex {
    final val X1: enum_pointIndex = 0.toUInt
    final val Y1: enum_pointIndex = 1.toUInt
    final val X2: enum_pointIndex = 2.toUInt
    final val Y2: enum_pointIndex = 3.toUInt
  }

  type enum_struct_op = native.CUnsignedInt
  object enum_struct_op {
    final val STRUCT_SET: enum_struct_op = 0.toUInt
    final val STRUCT_TEST: enum_struct_op = 1.toUInt
  }

  type struct_point = native.CStruct2[native.CInt, native.CInt]
  type point = struct_point
  type struct_points = native.CStruct2[struct_point, point]
  type point_s = native.Ptr[struct_point]
  type struct_bigStruct = native.CArray[Byte, native.Nat.Digit[native.Nat._1, native.Nat.Digit[native.Nat._1, native.Nat._2]]]
  type struct_anonymous_0 = native.CStruct2[native.CChar, native.CInt]
  type struct_anonymous_1 = native.CStruct1[native.CInt]
  type struct_structWithAnonymousStruct = native.CStruct3[native.CInt, struct_anonymous_0, struct_anonymous_1]
  type struct_packedStruct = native.CStruct1[native.CChar]
  type struct_bitFieldStruct = native.CArray[Byte, native.Nat._2]
  type struct_bitFieldOffsetDivByEight = native.CArray[Byte, native.Nat._4]
  def setPoints(points: native.Ptr[struct_points], x1: native.CInt, y1: native.CInt, x2: native.CInt, y2: native.CInt): Unit = native.extern
  def getPoint(points: native.Ptr[struct_points], pointIndex: enum_pointIndex): native.CInt = native.extern
  def createPoint(): native.Ptr[struct_point] = native.extern
  def getBigStructSize(): native.CInt = native.extern
  def getCharFromAnonymousStruct(s: native.Ptr[struct_structWithAnonymousStruct]): native.CChar = native.extern
  def getIntFromAnonymousStruct(s: native.Ptr[struct_structWithAnonymousStruct]): native.CInt = native.extern
  def getFieldOfUnnamedStruct(s: native.Ptr[struct_structWithAnonymousStruct]): native.CInt = native.extern
  def struct_test_long(s: native.Ptr[struct_bigStruct], op: enum_struct_op, value: native.CLong): native.CInt = native.extern
  def struct_test_double(s: native.Ptr[struct_bigStruct], op: enum_struct_op, value: native.CDouble): native.CInt = native.extern
  def struct_test_point(s: native.Ptr[struct_bigStruct], op: enum_struct_op, value: native.Ptr[struct_point]): native.CInt = native.extern

  object implicits {
    implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
      def x: native.CInt = !p._1
      def x_=(value: native.CInt): Unit = !p._1 = value
      def y: native.CInt = !p._2
      def y_=(value: native.CInt): Unit = !p._2 = value
    }

    implicit class struct_points_ops(val p: native.Ptr[struct_points]) extends AnyVal {
      def p1: native.Ptr[struct_point] = p._1
      def p1_=(value: native.Ptr[struct_point]): Unit = !p._1 = !value
      def p2: native.Ptr[point] = p._2
      def p2_=(value: native.Ptr[point]): Unit = !p._2 = !value
    }

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

    implicit class struct_anonymous_0_ops(val p: native.Ptr[struct_anonymous_0]) extends AnyVal {
      def c: native.CChar = !p._1
      def c_=(value: native.CChar): Unit = !p._1 = value
      def i: native.CInt = !p._2
      def i_=(value: native.CInt): Unit = !p._2 = value
    }

    implicit class struct_anonymous_1_ops(val p: native.Ptr[struct_anonymous_1]) extends AnyVal {
      def b: native.CInt = !p._1
      def b_=(value: native.CInt): Unit = !p._1 = value
    }

    implicit class struct_structWithAnonymousStruct_ops(val p: native.Ptr[struct_structWithAnonymousStruct]) extends AnyVal {
      def a: native.CInt = !p._1
      def a_=(value: native.CInt): Unit = !p._1 = value
      def anonymousStruct: native.Ptr[struct_anonymous_0] = p._2
      def anonymousStruct_=(value: native.Ptr[struct_anonymous_0]): Unit = !p._2 = !value
      def unnamed_0: native.Ptr[struct_anonymous_1] = p._3
      def unnamed_0_=(value: native.Ptr[struct_anonymous_1]): Unit = !p._3 = !value
    }
  }

  object struct_point {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]
    def apply(x: native.CInt, y: native.CInt)(implicit z: native.Zone): native.Ptr[struct_point] = {
      val ptr = native.alloc[struct_point]
      ptr.x = x
      ptr.y = y
      ptr
    }
  }

  object struct_points {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_points] = native.alloc[struct_points]
    def apply(p1: native.Ptr[struct_point], p2: native.Ptr[point])(implicit z: native.Zone): native.Ptr[struct_points] = {
      val ptr = native.alloc[struct_points]
      ptr.p1 = p1
      ptr.p2 = p2
      ptr
    }
  }

  object struct_bigStruct {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_bigStruct] = native.alloc[struct_bigStruct]
    def apply(one: native.CLong, two: native.CChar, three: native.CInt, four: native.CFloat, five: native.CDouble, six: native.Ptr[struct_point], seven: native.Ptr[struct_point], eight: native.CInt, nine: native.CInt, ten: native.CInt, eleven: native.CInt, twelve: native.CInt, thirteen: native.CInt, fourteen: native.CInt, fifteen: native.CInt, sixteen: native.CInt, seventeen: native.CInt, eighteen: native.CInt, nineteen: native.CInt, twenty: native.CInt, twentyOne: native.CInt, twentyTwo: native.CInt, twentyThree: native.CInt)(implicit z: native.Zone): native.Ptr[struct_bigStruct] = {
      val ptr = native.alloc[struct_bigStruct]
      ptr.one = one
      ptr.two = two
      ptr.three = three
      ptr.four = four
      ptr.five = five
      ptr.six = six
      ptr.seven = seven
      ptr.eight = eight
      ptr.nine = nine
      ptr.ten = ten
      ptr.eleven = eleven
      ptr.twelve = twelve
      ptr.thirteen = thirteen
      ptr.fourteen = fourteen
      ptr.fifteen = fifteen
      ptr.sixteen = sixteen
      ptr.seventeen = seventeen
      ptr.eighteen = eighteen
      ptr.nineteen = nineteen
      ptr.twenty = twenty
      ptr.twentyOne = twentyOne
      ptr.twentyTwo = twentyTwo
      ptr.twentyThree = twentyThree
      ptr
    }
  }

  object struct_anonymous_0 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_anonymous_0] = native.alloc[struct_anonymous_0]
    def apply(c: native.CChar, i: native.CInt)(implicit z: native.Zone): native.Ptr[struct_anonymous_0] = {
      val ptr = native.alloc[struct_anonymous_0]
      ptr.c = c
      ptr.i = i
      ptr
    }
  }

  object struct_anonymous_1 {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_anonymous_1] = native.alloc[struct_anonymous_1]
    def apply(b: native.CInt)(implicit z: native.Zone): native.Ptr[struct_anonymous_1] = {
      val ptr = native.alloc[struct_anonymous_1]
      ptr.b = b
      ptr
    }
  }

  object struct_structWithAnonymousStruct {
    import implicits._
    def apply()(implicit z: native.Zone): native.Ptr[struct_structWithAnonymousStruct] = native.alloc[struct_structWithAnonymousStruct]
    def apply(a: native.CInt, anonymousStruct: native.Ptr[struct_anonymous_0], unnamed_0: native.Ptr[struct_anonymous_1])(implicit z: native.Zone): native.Ptr[struct_structWithAnonymousStruct] = {
      val ptr = native.alloc[struct_structWithAnonymousStruct]
      ptr.a = a
      ptr.anonymousStruct = anonymousStruct
      ptr.unnamed_0 = unnamed_0
      ptr
    }
  }
}
