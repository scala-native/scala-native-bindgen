package org.example

import scala.scalanative._
import scala.scalanative.native._

@native.link("vector")
@native.extern
object vector {
  type struct_point = native.CStruct2[native.CFloat, native.CFloat]
  type struct_vector = native.CStruct2[struct_point, struct_point]
  def add(v1: native.Ptr[struct_vector], v2: native.Ptr[struct_vector]): native.Ptr[struct_vector] = native.extern
}

import vector._

object vectorHelpers {

  implicit class struct_point_ops(val p: native.Ptr[struct_point]) extends AnyVal {
    def x: native.CFloat = !p._1
    def x_=(value: native.CFloat): Unit = !p._1 = value
    def y: native.CFloat = !p._2
    def y_=(value: native.CFloat): Unit = !p._2 = value
  }

  def struct_point()(implicit z: native.Zone): native.Ptr[struct_point] = native.alloc[struct_point]

  implicit class struct_vector_ops(val p: native.Ptr[struct_vector]) extends AnyVal {
    def a: native.Ptr[struct_point] = p._1
    def a_=(value: native.Ptr[struct_point]): Unit = !p._1 = !value
    def b: native.Ptr[struct_point] = p._2
    def b_=(value: native.Ptr[struct_point]): Unit = !p._2 = !value
  }

  def struct_vector()(implicit z: native.Zone): native.Ptr[struct_vector] = native.alloc[struct_vector]
}
