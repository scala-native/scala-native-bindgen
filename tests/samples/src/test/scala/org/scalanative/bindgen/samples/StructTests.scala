package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.StructHelpers._

object StructTests extends TestSuite {
  val tests = Tests {
    'getPoint - {
      val point = Struct.getPoint()
      assert(point.x == 10)
      assert(point.y == 20)

      point.x_=(11)
      assert(point.x == 11)
    }

    'bigStructSize - {
      assert(Struct.getBigStructSize() == sizeof[Struct.struct_bigStruct])
    }

    'innerAnonymousStruct - {
      type struct_anonymousStruct = CStruct2[CChar, CInt]
      Zone { implicit zone =>
        val anonymousStruct: Ptr[struct_anonymousStruct] = alloc[struct_anonymousStruct]
        !anonymousStruct._1 = 'a'
        !anonymousStruct._2 = 42

        val structWithAnonymousStruct = struct_structWithAnonymousStruct()
        val array: Ptr[CArray[Byte, Nat._8]] = anonymousStruct.cast[Ptr[CArray[Byte, Nat._8]]]
        !structWithAnonymousStruct._2 = !array // works
        // structWithAnonymousStruct.anonymousStruct_=(!array) // fixme: fails
        // val s = structWithAnonymousStruct.anonymousStruct // fixme: fails

        assert('a' == Struct.getCharFromAnonymousStruct(structWithAnonymousStruct))
        assert(42 == Struct.getIntFromAnonymousStruct(structWithAnonymousStruct))
      }
    }
  }
}
