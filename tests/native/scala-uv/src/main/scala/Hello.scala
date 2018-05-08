import scalanative._
import scalanative.native._, stdlib._, stdio._


object Hello extends App {
  val time = stackalloc[CDouble]
  uv.uv_uptime(time)
  println("uptime: " + !time)


  def on_new_connection(server: Ptr[uv.uv_stream_t], status: CInt): Unit = {
    println(s"it works ! Stauts: $status")
    exit(0)
  }

  val callback = CFunctionPtr.fromFunction2(on_new_connection)

  native.Zone { z =>

    val loop = uv.uv_default_loop()

    val addr = malloc(sizeof[in.struct_sockaddr_in]).cast[Ptr[in.struct_sockaddr_in]]
    uv.uv_ip4_addr(toCString("127.0.0.1")(z), 8080, addr)

    val server = malloc(sizeof[uv.uv_tcp_t]).cast[Ptr[uv.uv_tcp_t]]

    val flags: Int = 0;


    uv.uv_tcp_init(loop, server)
    uv.uv_tcp_bind(server, addr.cast[Ptr[in.struct_sockaddr]], flags.toUInt)
    uv.uv_listen(server.asInstanceOf[Ptr[uv.uv_stream_t]], 128, callback)
    uv.uv_run(loop, uvEnums.enum_uv_run_mode_UV_RUN_DEFAULT)


  }



}