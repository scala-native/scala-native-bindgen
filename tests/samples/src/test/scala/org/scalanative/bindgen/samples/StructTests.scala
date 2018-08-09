package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.StructHelpers._

object StructTests extends TestSuite {
  val tests = Tests {
    'point - {
      val point = Struct.createPoint()
      assert(point.x == 10)
      assert(point.y == 20)

      point.x = 11
      assert(point.x == 11)
    }

    'bigStructSize - {
      assert(Struct.getBigStructSize() == sizeof[Struct.struct_bigStruct])
    }

    'getFieldsOfInnerStruct - {
      Zone { implicit zone =>
        val points = struct_points()
        Struct.setPoints(points, 1, 2, 3, 4)
        assert(1 == points.p1.x)
        assert(2 == points.p1.y)
        assert(3 == points.p2.x)
        assert(4 == points.p2.y)
      }
    }

    'setFieldsOfInnerStruct - {
      Zone { implicit zone =>
        val points = struct_points()
        points.p1.x = 1
        points.p1.y = 2
        points.p2.x = 3
        points.p2.y = 4
        assert(1 == Struct.getPoint(points, Struct.enum_pointIndex.X1))
        assert(2 == Struct.getPoint(points, Struct.enum_pointIndex.Y1))
        assert(3 == Struct.getPoint(points, Struct.enum_pointIndex.X2))
        assert(4 == Struct.getPoint(points, Struct.enum_pointIndex.Y2))
      }
    }

    'innerAnonymousStruct - {
      type struct_anonymousStruct = CStruct2[CChar, CInt]
      Zone { implicit zone =>
        val anonymousStruct: Ptr[struct_anonymousStruct] = alloc[struct_anonymousStruct]
        !anonymousStruct._1 = 'a'
        !anonymousStruct._2 = 42

        val structWithAnonymousStruct = struct_structWithAnonymousStruct()
        structWithAnonymousStruct.anonymousStruct = anonymousStruct

        assert('a' == Struct.getCharFromAnonymousStruct(structWithAnonymousStruct))
        assert(42 == Struct.getIntFromAnonymousStruct(structWithAnonymousStruct))
      }
    }

    'getFieldOfBigStruct - {
      Zone { implicit zone: Zone =>
        val structPtr = alloc[Struct.struct_bigStruct]
        for (value <- Seq(Long.MinValue, -1, 0, 1, Long.MaxValue)) {
          Struct.struct_test_long(structPtr, Struct.enum_struct_op.STRUCT_SET, value)
          assert(structPtr.one == value)
        }

        for (value <- Seq(Double.MinValue, -1, 0, 1, Double.MaxValue)) {
          Struct.struct_test_double(structPtr, Struct.enum_struct_op.STRUCT_SET, value)
          assert(structPtr.five == value)
        }

        val pointPtr = alloc[Struct.point]
        pointPtr.x = 5
        pointPtr.y = 10

        Struct.struct_test_point(structPtr, Struct.enum_struct_op.STRUCT_SET, pointPtr)
        assert(structPtr.six.x == pointPtr.x)
        assert(structPtr.six.y == pointPtr.y)
      }
    }

    'setFieldOfBigStruct - {
      Zone { implicit zone: Zone =>
        val structPtr = alloc[Struct.struct_bigStruct]
        for (value <- Seq(Long.MinValue, -1, 0, 1, Long.MaxValue)) {
          structPtr.one = value
          assert(Struct.struct_test_long(structPtr, Struct.enum_struct_op.STRUCT_TEST, value) == 1)
        }

        for (value <- Seq(Double.MinValue, -1, 0, 1, Double.MaxValue)) {
          structPtr.five = value
          assert(Struct.struct_test_double(structPtr, Struct.enum_struct_op.STRUCT_TEST, value) == 1)
        }

        val pointPtr = alloc[Struct.point]
        pointPtr.x = 5
        pointPtr.y = 10
        structPtr.six = pointPtr
        assert(Struct.struct_test_point(structPtr, Struct.enum_struct_op.STRUCT_TEST, pointPtr) == 1)
      }
    }
  }
}
