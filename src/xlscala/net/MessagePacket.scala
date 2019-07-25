package xlscala.net

import aether._
 
 
final class MessagePacket {
	protected var _buffer = PackedBuffer.allocate(Defines.BUFFSIZE)
	 
	def clear = _buffer.position = 0
	
	def position = _buffer.position
	def position_=(p :Int) = _buffer.position = p
	def position(p :Int) = _buffer.position = p
	 
	def data :Array[Byte] = _buffer.data
	def size = _buffer.position

	@inline
	protected def insert(data :Byte) = _buffer.putByte(data)
	
	@inline
	protected def inserts(data :Array[Byte]) = _buffer.putBytes(data)
	
	def getBytes :Array[Byte] = {
		var data = new Array[Byte](size)
		System.arraycopy(_buffer.data, 0, data, 0, size)
		return data
	}
	
	def putError(msg :String) {
		_buffer.putByte('e')
		_buffer.putInt(msg.size)
		_buffer.putBytes(msg.getBytes)
	}
	
	def +=(v :Any) {
		v match {
			case v :Float => _buffer.putByte('f'); _buffer.putFloat(v)
			case v :Double => _buffer.putByte('d'); _buffer.putDouble(v)
			case v :Int => _buffer.putByte('i'); _buffer.putInt(v)
			case v :Long => _buffer.putByte('l'); _buffer.putLong(v)
			case v :Boolean => _buffer.putByte('o'); if (v) _buffer.putByte(1) else _buffer.putByte(0)
			case v :Byte => _buffer.putByte('b'); _buffer.putByte(v)
			case v :Char => _buffer.putByte('c'); _buffer.putChar(v)
			case v :Short => _buffer.putByte('t'); _buffer.putShort(v)
			case v :Unit => _buffer.putByte('u');
			case v :String => _buffer.putByte('s'); _buffer.putInt(v.size); _buffer.putBytes(v.getBytes)
			case null => _buffer.putByte('n')
			//case v :Array[_] => ++=(v)
			//case v :Iterable[_] => ++=(v)
			case _ => sys.error("unsuported type by MessagePacket: " + v)
		}
	}
	
	def ++=(vs :Iterable[Any]) {
		_buffer.putByte('*')
		_buffer.putInt(vs.size)
		for (v <- vs) +=(v) 
	}
	
	def getError :Option[String] = {
		_buffer.getByte match {
			case 'e' => {
				var size = _buffer.getInt
				var ret = _buffer.getBytes(size)
				return Some(new String(ret))
			}
			case _ => {
				_buffer.position = _buffer.position - 1
				return None
			}
		}
	}
	
	def toAny : Any = {
		var v = _buffer.getByte
		v match {
			case 'f' => _buffer.getFloat
			case 'd' => _buffer.getDouble
			case 'i' => _buffer.getInt
			case 'l' => _buffer.getLong
			case 'o' => _buffer.getByte == 1
			case 'b' => _buffer.getByte
			case 'c' => _buffer.getChar
			case 'u' => Unit
			case 'n' => null
			case 's' => {
				var size = _buffer.getInt
				var ret = _buffer.getBytes(size)
				return new String(ret)
			}
			case 'e' => {
				var size = _buffer.getInt
				var ret = _buffer.getBytes(size)
				error(new String(ret))
			}
			case '*' => {
				var size = _buffer.getInt
				var ret = new Array[Any](size)
				for (i <- 0 until size) ret(i) = toAny
				return ret
			}
			case _ => sys.error("unsuported type by MessagePacket: " + v)
		}
	}

}

object MessagePacket {
	
	def apply() = new MessagePacket
	
	def main(args :Array[String]) {
		
		println("KKK")
		
		var b = new MessagePacket
		
		b += "TOTO"
		b.data
		b.position = 0
		println(b.toAny, b.size)
		
		
		/*{
		var begin = System.currentTimeMillis
		for (i <- 0 until 20) {
			var a = new MessagePacket
			for (i <- 0 until N) {
				a += i
			}
		}
		var end = System.currentTimeMillis
		println((end-begin) / 1000.0)
		}*/
	
		
		//a += "ohohoho"
		//b += "ohohoho"
		/*a += "Tosfsdfsdto"
		a += 5454
		a += 5.0
		a += true
		a += List(5.0, 554.0)
		a += 5.0
		a.insertError("TOTO")
		a += List(null, null)
		a += 5.0*/
		//println(a.data.toList)
		//println(b.data.toList)
		
		//a.position(0)
		//println(a.toAny)
		/*println(a.toAny)
		println(a.toAny)
		println(a.toAny)
		println(a.toAny)
		println(a.toAny)
		println(a.toAny)
		println(a.toAny)
		println(a.toAny)*/

	}

}
	 
