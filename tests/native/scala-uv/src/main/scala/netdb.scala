import scala.scalanative._
import scala.scalanative.native.Nat._
import stdint._
import types._
import in._

@native.extern
object netdb {
  type struct_rpcent = native.CStruct3[native.Ptr[native.CChar],native.Ptr[native.Ptr[native.CChar]],native.CInt]
  def setrpcent(__stayopen: native.CInt): Unit = native.extern
  def endrpcent(): Unit = native.extern
  def getrpcbyname(__name: native.Ptr[native.CChar]): native.Ptr[struct_rpcent] = native.extern
  def getrpcbynumber(__number: native.CInt): native.Ptr[struct_rpcent] = native.extern
  def getrpcent(): native.Ptr[struct_rpcent] = native.extern
  def getrpcbyname_r(__name: native.Ptr[native.CChar], __result_buf: native.Ptr[struct_rpcent], __buffer: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_rpcent]]): native.CInt = native.extern
  def getrpcbynumber_r(__number: native.CInt, __result_buf: native.Ptr[struct_rpcent], __buffer: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_rpcent]]): native.CInt = native.extern
  def getrpcent_r(__result_buf: native.Ptr[struct_rpcent], __buffer: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_rpcent]]): native.CInt = native.extern
  type struct_netent = native.CStruct4[native.Ptr[native.CChar],native.Ptr[native.Ptr[native.CChar]],native.CInt,uint32_t]
  def __h_errno_location(): native.Ptr[native.CInt] = native.extern
  def herror(__str: native.Ptr[native.CChar]): Unit = native.extern
  def hstrerror(__err_num: native.CInt): native.Ptr[native.CChar] = native.extern
  type struct_hostent = native.CStruct5[native.Ptr[native.CChar],native.Ptr[native.Ptr[native.CChar]],native.CInt,native.CInt,native.Ptr[native.Ptr[native.CChar]]]
  def sethostent(__stay_open: native.CInt): Unit = native.extern
  def endhostent(): Unit = native.extern
  def gethostent(): native.Ptr[struct_hostent] = native.extern
  def gethostbyaddr(__addr: native.Ptr[Byte], __len: __socklen_t, __type: native.CInt): native.Ptr[struct_hostent] = native.extern
  def gethostbyname(__name: native.Ptr[native.CChar]): native.Ptr[struct_hostent] = native.extern
  def gethostbyname2(__name: native.Ptr[native.CChar], __af: native.CInt): native.Ptr[struct_hostent] = native.extern
  def gethostent_r(__result_buf: native.Ptr[struct_hostent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_hostent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  def gethostbyaddr_r(__addr: native.Ptr[Byte], __len: __socklen_t, __type: native.CInt, __result_buf: native.Ptr[struct_hostent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_hostent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  def gethostbyname_r(__name: native.Ptr[native.CChar], __result_buf: native.Ptr[struct_hostent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_hostent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  def gethostbyname2_r(__name: native.Ptr[native.CChar], __af: native.CInt, __result_buf: native.Ptr[struct_hostent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_hostent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  def setnetent(__stay_open: native.CInt): Unit = native.extern
  def endnetent(): Unit = native.extern
  def getnetent(): native.Ptr[struct_netent] = native.extern
  def getnetbyaddr(__net: uint32_t, __type: native.CInt): native.Ptr[struct_netent] = native.extern
  def getnetbyname(__name: native.Ptr[native.CChar]): native.Ptr[struct_netent] = native.extern
  def getnetent_r(__result_buf: native.Ptr[struct_netent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_netent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  def getnetbyaddr_r(__net: uint32_t, __type: native.CInt, __result_buf: native.Ptr[struct_netent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_netent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  def getnetbyname_r(__name: native.Ptr[native.CChar], __result_buf: native.Ptr[struct_netent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_netent]], __h_errnop: native.Ptr[native.CInt]): native.CInt = native.extern
  type struct_servent = native.CStruct4[native.Ptr[native.CChar],native.Ptr[native.Ptr[native.CChar]],native.CInt,native.Ptr[native.CChar]]
  def setservent(__stay_open: native.CInt): Unit = native.extern
  def endservent(): Unit = native.extern
  def getservent(): native.Ptr[struct_servent] = native.extern
  def getservbyname(__name: native.Ptr[native.CChar], __proto: native.Ptr[native.CChar]): native.Ptr[struct_servent] = native.extern
  def getservbyport(__port: native.CInt, __proto: native.Ptr[native.CChar]): native.Ptr[struct_servent] = native.extern
  def getservent_r(__result_buf: native.Ptr[struct_servent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_servent]]): native.CInt = native.extern
  def getservbyname_r(__name: native.Ptr[native.CChar], __proto: native.Ptr[native.CChar], __result_buf: native.Ptr[struct_servent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_servent]]): native.CInt = native.extern
  def getservbyport_r(__port: native.CInt, __proto: native.Ptr[native.CChar], __result_buf: native.Ptr[struct_servent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_servent]]): native.CInt = native.extern
  type struct_protoent = native.CStruct3[native.Ptr[native.CChar],native.Ptr[native.Ptr[native.CChar]],native.CInt]
  def setprotoent(__stay_open: native.CInt): Unit = native.extern
  def endprotoent(): Unit = native.extern
  def getprotoent(): native.Ptr[struct_protoent] = native.extern
  def getprotobyname(__name: native.Ptr[native.CChar]): native.Ptr[struct_protoent] = native.extern
  def getprotobynumber(__proto: native.CInt): native.Ptr[struct_protoent] = native.extern
  def getprotoent_r(__result_buf: native.Ptr[struct_protoent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_protoent]]): native.CInt = native.extern
  def getprotobyname_r(__name: native.Ptr[native.CChar], __result_buf: native.Ptr[struct_protoent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_protoent]]): native.CInt = native.extern
  def getprotobynumber_r(__proto: native.CInt, __result_buf: native.Ptr[struct_protoent], __buf: native.Ptr[native.CChar], __buflen: native.CSize, __result: native.Ptr[native.Ptr[struct_protoent]]): native.CInt = native.extern
  def setnetgrent(__netgroup: native.Ptr[native.CChar]): native.CInt = native.extern
  def endnetgrent(): Unit = native.extern
  def getnetgrent(__hostp: native.Ptr[native.Ptr[native.CChar]], __userp: native.Ptr[native.Ptr[native.CChar]], __domainp: native.Ptr[native.Ptr[native.CChar]]): native.CInt = native.extern
  def innetgr(__netgroup: native.Ptr[native.CChar], __host: native.Ptr[native.CChar], __user: native.Ptr[native.CChar], __domain: native.Ptr[native.CChar]): native.CInt = native.extern
  def getnetgrent_r(__hostp: native.Ptr[native.Ptr[native.CChar]], __userp: native.Ptr[native.Ptr[native.CChar]], __domainp: native.Ptr[native.Ptr[native.CChar]], __buffer: native.Ptr[native.CChar], __buflen: native.CSize): native.CInt = native.extern
  def rcmd(__ahost: native.Ptr[native.Ptr[native.CChar]], __rport: native.CUnsignedShort, __locuser: native.Ptr[native.CChar], __remuser: native.Ptr[native.CChar], __cmd: native.Ptr[native.CChar], __fd2p: native.Ptr[native.CInt]): native.CInt = native.extern
  def rcmd_af(__ahost: native.Ptr[native.Ptr[native.CChar]], __rport: native.CUnsignedShort, __locuser: native.Ptr[native.CChar], __remuser: native.Ptr[native.CChar], __cmd: native.Ptr[native.CChar], __fd2p: native.Ptr[native.CInt], __af: sa_family_t): native.CInt = native.extern
  def rexec(__ahost: native.Ptr[native.Ptr[native.CChar]], __rport: native.CInt, __name: native.Ptr[native.CChar], __pass: native.Ptr[native.CChar], __cmd: native.Ptr[native.CChar], __fd2p: native.Ptr[native.CInt]): native.CInt = native.extern
  def rexec_af(__ahost: native.Ptr[native.Ptr[native.CChar]], __rport: native.CInt, __name: native.Ptr[native.CChar], __pass: native.Ptr[native.CChar], __cmd: native.Ptr[native.CChar], __fd2p: native.Ptr[native.CInt], __af: sa_family_t): native.CInt = native.extern
  def ruserok(__rhost: native.Ptr[native.CChar], __suser: native.CInt, __remuser: native.Ptr[native.CChar], __locuser: native.Ptr[native.CChar]): native.CInt = native.extern
  def ruserok_af(__rhost: native.Ptr[native.CChar], __suser: native.CInt, __remuser: native.Ptr[native.CChar], __locuser: native.Ptr[native.CChar], __af: sa_family_t): native.CInt = native.extern
  def iruserok(__raddr: uint32_t, __suser: native.CInt, __remuser: native.Ptr[native.CChar], __locuser: native.Ptr[native.CChar]): native.CInt = native.extern
  def iruserok_af(__raddr: native.Ptr[Byte], __suser: native.CInt, __remuser: native.Ptr[native.CChar], __locuser: native.Ptr[native.CChar], __af: sa_family_t): native.CInt = native.extern
  def rresvport(__alport: native.Ptr[native.CInt]): native.CInt = native.extern
  def rresvport_af(__alport: native.Ptr[native.CInt], __af: sa_family_t): native.CInt = native.extern
  type struct_addrinfo = native.CStruct8[native.CInt,native.CInt,native.CInt,native.CInt,socklen_t,native.Ptr[struct_sockaddr],native.Ptr[native.CChar],native.Ptr[native.CArray[Byte, Digit[_3, Digit[_8, _4]]]]]
  def getaddrinfo(__name: native.Ptr[native.CChar], __service: native.Ptr[native.CChar], __req: native.Ptr[struct_addrinfo], __pai: native.Ptr[native.Ptr[struct_addrinfo]]): native.CInt = native.extern
  def freeaddrinfo(__ai: native.Ptr[struct_addrinfo]): Unit = native.extern
  def gai_strerror(__ecode: native.CInt): native.Ptr[native.CChar] = native.extern
  def getnameinfo(__sa: native.Ptr[struct_sockaddr], __salen: socklen_t, __host: native.Ptr[native.CChar], __hostlen: socklen_t, __serv: native.Ptr[native.CChar], __servlen: socklen_t, __flags: native.CInt): native.CInt = native.extern
}
