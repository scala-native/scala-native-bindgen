package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.StructHelpers._
import org.scalanative.bindgen.samples.StructEnums._

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
        assert(1 == Struct.getPoint(points, enum_pointIndex_X1))
        assert(2 == Struct.getPoint(points, enum_pointIndex_Y1))
        assert(3 == Struct.getPoint(points, enum_pointIndex_X2))
        assert(4 == Struct.getPoint(points, enum_pointIndex_Y2))
      }
    }

    'innerAnonymousStruct - {
      type struct_anonymousStruct = CStruct2[CChar, CInt]
      Zone { implicit zone =>
        val anonymousStruct: Ptr[struct_anonymousStruct] = alloc[struct_anonymousStruct]
        !anonymousStruct._1 = 'a'
        !anonymousStruct._2 = 42

        val structWithAnonymousStruct = struct_structWithAnonymousStruct()
        val array = anonymousStruct.cast[Ptr[CArray[Byte, Nat._8]]]
        structWithAnonymousStruct.anonymousStruct = array

        assert('a' == Struct.getCharFromAnonymousStruct(structWithAnonymousStruct))
        assert(42 == Struct.getIntFromAnonymousStruct(structWithAnonymousStruct))
      }
    }
  }
}
