package org.scalanative.bindgen.docs

import org.scalatest.FunSpec

class VectorSpec extends FunSpec {
  describe("vector") {
    it("using generated bindings") {
      //#step-1
      import scala.scalanative.native.Zone
      import org.example.vector
      import org.example.vector.implicits._

      Zone { implicit zone =>
        val vec1p1 = struct_point()
        val vec1p2 = struct_point()
        //#step-1
        //#step-2
        vec1p1.x = 0
        vec1p1.y = 1

        vec1p2.x = 6
        vec1p2.y = 3
        //#step-2
        //#step-3
        val vec1 = struct_vector()
        vec1.a = vec1p1
        vec1.b = vec1p2
        //#step-3
        val vec2 = vec1
        //#step-4
        val vec3 = vector.add(vec1, vec2)
        println(s"(${vec3.a.x}, ${vec3.a.y}), (${vec3.b.x}, ${vec3.b.y})")
        //#step-4
        assert(vec3.a.x == vec1.a.x + vec2.a.x)
      //#step-1
      }
      //#step-1
    }
  }
}
