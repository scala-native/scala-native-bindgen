package org.scalanative.bindgen.samples

import utest._
import scala.scalanative.native._
import org.scalanative.bindgen.samples.UnionHelpers._

object UnionTests extends TestSuite {
  val tests = Tests {
    'sizeof - {
      assert(Union.union_get_sizeof() == sizeof[Union.union_values])
    }

    'get - {
      Zone { implicit zone =>
        val unionPtr = alloc[Union.union_values]

        for (value <- Seq(Int.MinValue, -1, 0, 1, Int.MaxValue)) {
          Union.union_test_int(unionPtr,
                               UnionEnums.enum_union_op_UNION_SET,
                               value)
          assert(!unionPtr.i == value)
        }

        for (value <- Seq(Long.MinValue, -1, 0, 1, Long.MaxValue)) {
          Union.union_test_long(unionPtr,
                                UnionEnums.enum_union_op_UNION_SET,
                                value)
          assert(!unionPtr.l == value)
        }

        val llValues = Seq[CLongLong](
          Long.MinValue - 100, -1, 0, 1, Long.MaxValue + 100)
        for (value <- llValues) {
          Union.union_test_long_long(unionPtr,
                                     UnionEnums.enum_union_op_UNION_SET,
                                     value)
          assert(!unionPtr.ll == value)
        }

        for (value <- Seq(Double.MinValue, -1, 0, 1, Double.MaxValue)) {
          Union.union_test_double(unionPtr,
                                  UnionEnums.enum_union_op_UNION_SET,
                                  value)
          assert(!unionPtr.d == value)
        }

        for (value <- Seq("", "bindgen")) {
          val s = toCString(value)
          Union.union_test_string(unionPtr,
                                  UnionEnums.enum_union_op_UNION_SET,
                                  s)
          assert(fromCString(!unionPtr.s) == value)
        }

        Union.union_test_string(unionPtr,
                                UnionEnums.enum_union_op_UNION_SET,
                                null)
        assert(!unionPtr.s == null)
      }
    }

    'set - {
      Zone { implicit zone =>
        val unionPtr = alloc[Union.union_values]

        for (value <- Seq(Int.MinValue, -1, 0, 1, Int.MaxValue)) {
          !unionPtr.i = value
          assert(
            Union.union_test_int(unionPtr,
                                 UnionEnums.enum_union_op_UNION_TEST,
                                 value) == 1)
        }

        for (value <- Seq(Long.MinValue, -1, 0, 1, Long.MaxValue)) {
          !unionPtr.l = value
          assert(
            Union.union_test_long(unionPtr,
                                  UnionEnums.enum_union_op_UNION_TEST,
                                  value) == 1)
        }

        val llValues = Seq[CLongLong](
          Long.MinValue - 100, -1, 0, 1, Long.MaxValue + 100)
        for (value <- llValues) {
          !unionPtr.ll = value
          assert(
            Union.union_test_long_long(unionPtr,
                                       UnionEnums.enum_union_op_UNION_TEST,
                                       value) == 1)
        }

        for (value <- Seq(Double.MinValue, -1, 0, 1, Double.MaxValue)) {
          !unionPtr.d = value
          assert(
            Union.union_test_double(unionPtr,
                                    UnionEnums.enum_union_op_UNION_TEST,
                                    value) == 1)
        }

        for (value <- Seq("", "bindgen")) {
          val s = toCString(value)
          !unionPtr.s = s
          val expected = toCString(value)
          assert(
            Union.union_test_string(unionPtr,
                                    UnionEnums.enum_union_op_UNION_TEST,
                                    expected) == 1)
        }

        !unionPtr.s = null
        assert(
          Union.union_test_string(unionPtr,
                                  UnionEnums.enum_union_op_UNION_TEST,
                                  null) == 1)
      }
    }
  }
}
