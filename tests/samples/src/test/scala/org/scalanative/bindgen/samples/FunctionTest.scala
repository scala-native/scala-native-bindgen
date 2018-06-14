package org.scalanative.bindgen.samples

import utest._
import scalanative.native._

object FunctionTests extends TestSuite {
  val tests = Tests {
    'no_args - {
      assert(Function.no_args() == 42)
    }

    'void_arg - {
      assert(Function.void_arg() == 1.5)
    }

    'one_arg - {
      assert(Function.one_arg(42) == '*')
    }

    'two_args - {
      val result = Function.two_args(3.14.toFloat, 1024)
      val string = fromCString(result.cast[CString])
      assert(string == "3.14 1024")
    }

    'anonymous_args - {
      assert(Function.anonymous_args(1.5.toFloat, 42) == 43.5)
    }

    'variadic_args - {
      Zone { implicit z =>
        val result =
          Function.variadic_args(0.2, toCString("0123"), 1, 10, 100, 1000)
        assert(result == 3210.2)
      }
    }
  }
}
