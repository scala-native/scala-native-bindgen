package org.scalanative.bindgen.bindings.iconv.tests

import org.scalatest.FunSpec

class Sqlite3Spec extends FunSpec {
  describe("sqlite3") {
    it("should open DB and run query") {
      //#usage-example
      import org.scalanative.bindgen.bindings.sqlite3.sqlite3._
      import scala.scalanative.native._

      val db = stackalloc[Ptr[Ptr[sqlite3]]]

      val rc_open = sqlite3_open(":memory:", db)
      if (rc_open != defines.SQLITE_OK) {
        stdio.fprintf(stdio.stderr, "Can't open database: %s\n", sqlite3_errmsg(!db))
        sqlite3_close(!db)
        //#usage-example
        fail("Failed to open database")
        //#usage-example
      } else {
        val res = stackalloc[Ptr[Ptr[sqlite3_stmt]]
        val callback = 
        val rc_exec = sqlite3_exec(!db, "SELECT SQLITE_VERSION()", callback, 0, &zErrMsg)    
        if (rc_exec != defines.SQLITE_OK) {  
          fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db))
          sqlite3_close(!db)
          //#usage-example
          fail("SQL error")
          //#usage-example
        } else {
          sqlite3_close(!db)
        }
      }
      //#usage-example
    }
  }
}
