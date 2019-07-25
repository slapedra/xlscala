package xlscala.net


import java.io._
import java.net._
import java.awt._

import aether._

class SimpleSocket(val socket :Socket) {
	
	protected  val _input = new BufferedInputStream(socket.getInputStream)
	protected  val _output = new BufferedOutputStream(socket.getOutputStream)
	
	def close {
		_input.close
		_output.close
		socket.close
	}
	
	def getInetAddress = socket.getInetAddress
	
	protected def writeSize(v :Int) {
		_output.write((v & 0xff).toByte)
		_output.write(((v >> 8) & 0xff).toByte)
		_output.write(((v >> 16) & 0xff).toByte)
		_output.write(((v >> 24) & 0xff).toByte)
	}
	
	protected def readSize :Int = {
		var buf = new Array[Byte](4)
		var count = _input.read(buf, 0, 4)
		require(count == 4, "error reading size pacquet")
		var size = (((buf(0).toInt + 256) & 0xff) << 0) + 
				  (((buf(1).toInt + 256) & 0xff) << 8) + 
				  (((buf(2).toInt + 256) & 0xff) << 16) + 
				  (((buf(3).toInt + 256) & 0xff) << 24)
		return size
	}
	
	def write(data :Array[Byte], len :Int) {
		writeSize(len)
		_output.write(data, 0, len)
		_output.flush
	}
	
	def read(data :Array[Byte]) :Int = {
		var count = 0
		var break = false
		var size = readSize
		while (!break) {
			var l = _input.read(data, count, size - count)
			require(l != -1, "error reading data, connection reset by peer")
			count += l
			if (count == size) {
				break = true	
			} 
		}
		return count
	}
	
}

object SimpleSocket {
	def apply(sock :Socket) = new SimpleSocket(sock)
}