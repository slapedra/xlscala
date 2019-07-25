package xlscala.net

import java.io._
import java.net._
import java.awt._

import scala.concurrent._
import scala.concurrent.ExecutionContext.Implicits.global
import scala.collection.mutable.ArrayBuffer
import aether._ 


protected final class SocketSession(sock :Socket, protected var _call :Function2[MessagePacket, MessagePacket, Bool]) { 

	protected var _sock = SimpleSocket(sock)
	protected var _address :String = _sock.getInetAddress.getHostName
	protected var _stop = false
	protected var _msg = new MessagePacket
	protected var _rmsg = new MessagePacket
	
	def close = _stop = true
	
	def start {
		while (!_stop) {
			try {
				_msg.clear
				_rmsg.clear
				var size = _sock.read(_msg.data)
				if (size >= 0) {
					_msg.clear
					require(_call(_msg, _rmsg) == false, "server closed")
				}
			} catch { 
				case e :Throwable => { 
					_rmsg.clear
					_rmsg.putError(e.toString)
					try {
						_sock.write(_rmsg.data, _rmsg.size)
					} catch {
						case _ :Throwable =>
					}
					clear
					_stop = true
				}
			}
			try {
				_sock.write(_rmsg.data, _rmsg.size)
			} catch {
				case e :Throwable => {
					_rmsg.clear
					clear
					_stop = true
				}
			}
		}
	}
		
	protected def clear = {
		_sock.close
	}
}


final class SocketServer(protected var _port :Int) {
	protected var _server :ServerSocket = null
	
	def setTimeout(time :Int) = _server.setSoTimeout(time)
	
	def port = _port
	
	{
		var tryn = 0
		var tries = 10
		var port = _port
		do {
			try {
				_server = new ServerSocket(_port, 100)
				tryn = tries
			} catch {
				case  _ :Throwable => _port += Defines.PORT_INCREMENT
			}
			tryn += 1
		} while (tryn < tries)
		if (tryn == tries) error("error installing server on port: " + port)
	}

    def connect(call :Function2[MessagePacket, MessagePacket, Bool]) {
		var sock = _server.accept
		var session = new SocketSession(sock, call)
		var f = future {
			session.start
		}
    }

    def close {
    	_server.close
    }
  
}
 
object SocketServer {
	def apply(port :Int) = new SocketServer(port)
	
	def main(args :Array[String]) {
		
		var data = new Array[Real](1024*100)
		data(0) = 5.0
		var server = SocketServer(12345)
		while (true) {
			println("connect")
			server.connect(
				(a :MessagePacket, b :MessagePacket) => { 
					b += "Hello"
					println("receive", a.toAny)
					false
				} 
			)
		}
		server.close
	}
}

