package xlscala.net

object TOTO {
	def main(args :Array[String]) {
		println("KKKK")
		
		var buffer = PackedBuffer(200)
		buffer.putChar('a')
		buffer.putByte('b')
		buffer.putByte('c')
		buffer.putByte('d')
		buffer.putByte('a')
		buffer.putChar('b')
		buffer.putByte('c')
		buffer.putByte('d')
		buffer.position(0)
		println(buffer.getChar, buffer.getChar, buffer.getChar, buffer.getChar)
		println(buffer.getChar, buffer.getChar, buffer.getChar, buffer.getChar)
	}
}


class PackedBuffer(protected var _array :Array[Byte], protected val _canGrow :Boolean = false) { self => 
	protected var _position  = 0
	
	@inline
	def position = _position
	
	@inline
	def position_=(p :Int) = _position = p
	
	@inline
	def position(p :Int) = _position = p
	
	@inline
	def aray  = if (_canGrow) _array.slice(0, _position) else _array
	
	@inline
	def data = _array

	@inline
	protected def ensureAvail(index :Int, n :Int) {
        if (index + n > _array.size) {
        	if (_canGrow) {
        		var newSize = Math.max(index + n, _array.size * 2)
        		_array = java.util.Arrays.copyOf(_array, newSize)
        	} else {
        		sys.error("PackedBuffer does not have enough capacity: " + _array.size + " requested: " + (index + n))
        	}
        }
    }
	
	@inline
	protected def checkAvail(index :Int, n :Int) {
		require((index + n) <= _array.size, "PackedBuffer does not have enough capacity: " + _array.size + " requested: " + (index + n))
    }
	
	@inline
	def putByte(index :Int, v :Byte) :self.type = {
		ensureAvail(index, 1)
		PackedBufferUtils.putByte(_array, index, v)
		_position = index + 1
		return self
	}
	
	@inline
	def putChar(index :Int, v :Char) :self.type = {
		ensureAvail(index, 1)
		PackedBufferUtils.putChar(_array, index, v)
		_position = index + 1
		return self
	}
	
	@inline
	def putShort(index :Int, v :Short) :self.type = {
		ensureAvail(index, 2)
		PackedBufferUtils.putShort(_array, index, v)
		_position = index + 2
		return self
	}
	
	@inline
	def putInt(index :Int, v :Int) :self.type = {
		ensureAvail(index, 4)
		PackedBufferUtils.putInt(_array, index, v)
		_position = index + 4
		return self
	}
	
	@inline
	def putLong(index :Int, v :Long) :self.type = {
		ensureAvail(index, 8)
		PackedBufferUtils.putLong(_array, index, v)
		_position = index + 8
		return self
	}
	
	@inline
	def putFloat(index :Int, v :Float) :self.type = {
		ensureAvail(index, 4)
		PackedBufferUtils.putFloat(_array, index, v)
		_position = index + 4
		return self
	}
	
	@inline
	def putDouble(index :Int, v :Double) :self.type = {
		ensureAvail(index, 8)
		PackedBufferUtils.putDouble(_array, index, v)
		_position = index + 8
		return self
	}
	
	@inline
	def putBytes(index :Int, v :Array[Byte]) :self.type = {
		ensureAvail(index, v.size)
		System.arraycopy(v, 0, _array, index, v.size)
		_position = index + v.size
		return self
	}
	
	@inline
	def putByte(v :Byte) :self.type = putByte(_position, v)
	
	@inline
	def putChar(v :Char) :self.type = putChar(_position, v)
	
	@inline
	def putShort(v :Short) :self.type = putShort(_position, v)
	
	@inline
	def putInt(v :Int) :self.type = putInt(_position, v)
	
	@inline
	def putLong(v :Long) :self.type = putLong(_position, v)
	
	@inline
	def putFloat(v :Float) :self.type = putFloat(_position, v)
	
	@inline
	def putDouble(v :Double) :self.type = putDouble(_position, v)
	
	@inline
	def putBytes(v :Array[Byte]) :self.type = putBytes(_position, v)
	
	@inline
	def getByte(index :Int) :Byte = {
		checkAvail(index, 1)
		var v = PackedBufferUtils.getByte(_array, index)
		_position = index + 1
		return v
	}
	
	@inline
	def getBytes(index :Int, size :Int) :Array[Byte] = {
		checkAvail(index, size)
		var ret = new Array[Byte](size)
		System.arraycopy(_array, index, ret, 0, size)
		_position = index + size
		return ret
	}
	
	@inline
	def getChar(index :Int) :Char = {
		checkAvail(index, 1)
		var v = PackedBufferUtils.getChar(_array, index)
		_position = index + 1
		return v
	}
	
	@inline
	def getShort(index :Int) :Short = {
		checkAvail(index, 2)
		var v = PackedBufferUtils.getShort(_array, index)
		_position = index + 2
		return v
	}
	
	@inline
	def getInt(index :Int) :Int = {
		checkAvail(index, 4)
		var v = PackedBufferUtils.getInt(_array, index)
		_position = index + 4
		return v
	}
	
	@inline
	def getLong(index :Int) :Long = {
		checkAvail(index, 8)
		var v = PackedBufferUtils.getLong(_array, index)
		_position = index + 8
		return v
	}
	
	@inline
	def getFloat(index :Int) :Float = {
		checkAvail(index, 4)
		var v = PackedBufferUtils.getFloat(_array, index)
		_position = index + 4
		return v
	}
	
	@inline
	def getDouble(index :Int) :Double = {
		checkAvail(index, 8)
		var v = PackedBufferUtils.getDouble(_array, index)
		_position = index + 8
		return v
	}
	
	@inline
	def getBytes(size :Int) :Array[Byte] = getBytes(_position, size)
	
	@inline
	def getByte :Byte = getByte(_position)
	
	@inline
	def getChar :Char = getChar(_position)
	
	@inline
	def getShort :Short = getShort(_position)
	
	@inline
	def getInt :Int = getInt(_position)
	
	@inline
	def getLong :Long = getLong(_position)
	
	@inline
	def getFloat :Float = getFloat(_position)
	
	@inline
	def getDouble :Double = getDouble(_position)
}

object PackedBuffer {
	def apply(size :Int = 16) = new PackedBuffer(new Array[Byte](size), true)
	def allocate(size :Int) = new PackedBuffer(new Array[Byte](size), false)
	def wrap(array :Array[Byte]) = new PackedBuffer(array, false)
}


private object PackedBufferUtils {
	 
	@inline
	def getByte(buf :Array[Byte], pos :Int) :Byte = {
		return Unsafe().getByte(buf, (Unsafe.BYTEARRAY_OFFSET + pos).toLong)
		//return buf(pos)
	}

	@inline
	def putByte(buf :Array[Byte], pos :Int, v :Byte) {
		Unsafe().putByte(buf, (Unsafe.BYTEARRAY_OFFSET + pos).toLong, v)
		//buf(pos + 0) = v
	}
	
	@inline
	def getChar(buf :Array[Byte], pos :Int) :Char = {
		return Unsafe().getByte(buf, (Unsafe.CHARARRAY_OFFSET + pos).toLong).toChar
		//return buf(pos).toChar
	}

	@inline
	def putChar(buf :Array[Byte], pos :Int, v :Char) {
		Unsafe().putByte(buf, (Unsafe.CHARARRAY_OFFSET + pos).toLong, v.toByte)
		//buf(pos + 0) = (0xff & (v >> 0)).toByte
	}

	@inline
	def getShort(buf :Array[Byte], pos :Int) :Short = {
		return Unsafe().getShort(buf, (Unsafe.SHORTARRAY_OFFSET + pos).toLong)
		//return ( 	((buf(pos + 0) & 0xff) << 8) +
		//		 	((buf(pos + 1) & 0xff) << 0) ).toShort
	}

	@inline
	def putShort(buf :Array[Byte], pos :Int, v :Short) {
		Unsafe().putShort(buf, (Unsafe.SHORTARRAY_OFFSET + pos).toLong, v)
		//buf(pos + 0) = (0xff & (v >> 8)).toByte
        //buf(pos + 1) = (0xff & (v >> 0)).toByte
	}
	
	@inline
	def getInt(buf :Array[Byte], pos :Int) :Int = {
		return Unsafe().getInt(buf, (Unsafe.INTARRAY_OFFSET + pos).toLong)
		/*return ( 	((buf(pos + 0) & 0xff).toInt << 24) +
				 	((buf(pos + 1) & 0xff).toInt << 16) +
				 	((buf(pos + 2) & 0xff).toInt << 8) +
				 	((buf(pos + 3) & 0xff).toInt << 0) )*/
	}

	@inline
	def putInt(buf :Array[Byte], pos :Int, v :Int) {
		Unsafe().putInt(buf, (Unsafe.INTARRAY_OFFSET + pos).toLong, v)
		/*buf(pos + 0) = (0xff & (v >> 24)).toByte
        buf(pos + 1) = (0xff & (v >> 16)).toByte
        buf(pos + 2) = (0xff & (v >> 8)).toByte
        buf(pos + 3) = (0xff & (v >> 0)).toByte*/
	}
	
	@inline
	def getLong(buf :Array[Byte], pos :Int) :Long = {
		return Unsafe().getLong(buf, (Unsafe.LONGARRAY_OFFSET + pos).toLong)
		/*return ( 	((buf(pos + 0) & 0xff).toLong << 56) +
				 	((buf(pos + 1) & 0xff).toLong << 48) +
				 	((buf(pos + 2) & 0xff).toLong << 40) +
				 	((buf(pos + 3) & 0xff).toLong << 32) +
				 	((buf(pos + 4) & 0xff).toLong << 24) +
				 	((buf(pos + 5) & 0xff).toLong << 16) +
				 	((buf(pos + 6) & 0xff).toLong << 8) +
				 	((buf(pos + 7) & 0xff).toLong << 0) )*/
	}

	@inline
	def putLong(buf :Array[Byte], pos :Int, v :Long) {
		Unsafe().putLong(buf, (Unsafe.LONGARRAY_OFFSET + pos).toLong, v)
		/*buf(pos + 0) = (0xff & (v >> 56)).toByte
        buf(pos + 1) = (0xff & (v >> 48)).toByte
        buf(pos + 2) = (0xff & (v >> 40)).toByte
        buf(pos + 3) = (0xff & (v >> 32)).toByte
        buf(pos + 4) = (0xff & (v >> 24)).toByte
        buf(pos + 5) = (0xff & (v >> 16)).toByte
        buf(pos + 6) = (0xff & (v >> 8)).toByte
        buf(pos + 7) = (0xff & (v >> 0)).toByte*/
	}
	
	@inline
	def getFloat(buf :Array[Byte], pos :Int) :Float = {
		return Unsafe().getFloat(buf, (Unsafe.FLOATARRAY_OFFSET + pos).toLong)
		//return java.lang.Float.intBitsToFloat(getInt(buf, pos))
	}

	@inline
	def putFloat(buf :Array[Byte], pos :Int, v :Float) {
		Unsafe().putFloat(buf, (Unsafe.FLOATARRAY_OFFSET + pos).toLong, v)
		//putInt(buf, pos, java.lang.Float.floatToIntBits(v))
	}
	
	@inline
	def getDouble(buf :Array[Byte], pos :Int) :Double = {
		return Unsafe().getDouble(buf, (Unsafe.DOUBLEARRAY_OFFSET + pos).toLong)
		//return java.lang.Double.longBitsToDouble(getLong(buf, pos))
	}

	@inline
	def putDouble(buf :Array[Byte], pos :Int, v :Double) {
		Unsafe().putDouble(buf, (Unsafe.DOUBLEARRAY_OFFSET + pos).toLong, v)
		//putLong(buf, pos, java.lang.Double.doubleToLongBits(v))
	}
	
}
