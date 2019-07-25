package xlscala.net

import java.io._
import java.net._
import java.awt._

import scala.collection.mutable.ArrayBuffer
import scala.collection.immutable.List

import aether._

final class SocketClient(protected var _port :Int,  protected var _host :String) {
	protected var _sock :SimpleSocket = null

	try {
		_sock = SimpleSocket(new Socket(InetAddress.getByName(_host), _port))
	} catch {
		case _ :Throwable => error("error connecting to " + _host + " at port " + _port)
	}
	
	def apply(msg :MessagePacket, rmsg :MessagePacket) = {
		_sock.write(msg.data, msg.size)
		var count = _sock.read(rmsg.data)
		require(count >= 0, "invalid reply")
		rmsg.position(0)
	} 
	
	def close = {
		_sock.close
	}
}

object SocketClient {
	
	def apply(port :Int, host :String) = new SocketClient(port, host)
	
	def main(args :Array[String]) {
		println("LLLL")
		var data = new Array[Byte](1024*100)
		var begin = System.currentTimeMillis();
		var client = SocketClient(12345, "localhost")
		var msg = new MessagePacket
		var rmsg = new MessagePacket
		for (x <- 0 until 10) {
			println("x", x)
			msg.clear
			rmsg.clear
			msg += "hello"
			/*msg += 's'
			msg += 1
			msg += 2
			msg += 2
			msg += false
			msg += "__1+_2"
			msg += "_new Array[Byte](_size)"
			msg ++= List(0, x, 1024*100)
			msg ++= List(null, null, "size")
			msg ++= data
			msg ++= List()*/
		
			client(msg, rmsg)
			println(rmsg.toAny)
		}
		var end = System.currentTimeMillis();
		println("\t\t ... done in " + (end - begin) / 1000.0 + " seconds")
		client.close
	}
	
}
