package xlscala.net

object Unsafe {
	  
	val BOOLEANARRAY_OFFSET =  instance.arrayBaseOffset(classOf[Array[Boolean]])
	val BYTEARRAY_OFFSET =  instance.arrayBaseOffset(classOf[Array[Byte]])
	val CHARARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Char]])  
	val SHORTARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Short]]) 
	val INTARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Int]]) 
	val LONGARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Long]]) 
	val FLOATARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Float]]) 
	val DOUBLEARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Double]])
	val ANYARRAY_OFFSET = instance.arrayBaseOffset(classOf[Array[Any]])
	
	val BOOLEANARRAY_SCALE =  instance.arrayIndexScale(classOf[Array[Boolean]])
	val BYTEARRAY_SCALE =  instance.arrayIndexScale(classOf[Array[Byte]])
	val CHARARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Char]])  
	val SHORTARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Short]]) 
	val INTARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Int]]) 
	val LONGARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Long]]) 
	val FLOATARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Float]]) 
	val DOUBLEARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Double]])
	val ANYARRAY_SCALE = instance.arrayIndexScale(classOf[Array[Any]])
 
	protected lazy val instance: sun.misc.Unsafe = {
		val fld = classOf[sun.misc.Unsafe].getDeclaredField("theUnsafe")
		fld.setAccessible(true)
		fld.get(null).asInstanceOf[sun.misc.Unsafe]
  	}

	@inline
  	def apply(): sun.misc.Unsafe = instance
  	
  	@inline def getBoolean(array :Array[Boolean], index :Int) = instance.getBoolean(array, BOOLEANARRAY_OFFSET + index * BOOLEANARRAY_SCALE toLong)
  	@inline def getByte(array :Array[Byte], index :Int) = instance.getByte(array, BYTEARRAY_OFFSET + index * BYTEARRAY_SCALE toLong)
  	@inline def getChar(array :Array[Char], index :Int) = instance.getChar(array, CHARARRAY_OFFSET + index * CHARARRAY_SCALE toLong)
  	@inline def getShort(array :Array[Short], index :Int) = instance.getShort(array, SHORTARRAY_OFFSET + index * SHORTARRAY_SCALE toLong)
  	@inline def getInt(array :Array[Int], index :Int) = instance.getInt(array, INTARRAY_OFFSET + index * INTARRAY_SCALE toLong)
  	@inline def getLong(array :Array[Long], index :Int) = instance.getLong(array, LONGARRAY_OFFSET + index * LONGARRAY_SCALE toLong)
  	@inline def getFloat(array :Array[Float], index :Int) = instance.getFloat(array, FLOATARRAY_OFFSET + index * FLOATARRAY_SCALE toLong)
  	@inline def getDouble(array :Array[Double], index :Int) = instance.getDouble(array, DOUBLEARRAY_OFFSET + index * DOUBLEARRAY_SCALE toLong)
  	@inline def get[A](array :Array[A], index :Int) :A = instance.getObject(array, ANYARRAY_OFFSET + index * ANYARRAY_SCALE toLong).asInstanceOf[A]
  	
	@inline def putBoolean(array :Array[Boolean], index :Int, value :Boolean) = instance.putBoolean(array, BOOLEANARRAY_OFFSET + index * BOOLEANARRAY_SCALE toLong, value)
  	@inline def putByte(array :Array[Byte], index :Int, value :Byte) = instance.putByte(array, BYTEARRAY_OFFSET + index * BYTEARRAY_SCALE toLong, value)
  	@inline def putChar(array :Array[Char], index :Int, value :Char) = instance.putChar(array, CHARARRAY_OFFSET + index * CHARARRAY_SCALE toLong, value)
  	@inline def putShort(array :Array[Short], index :Int, value :Short) = instance.putShort(array, SHORTARRAY_OFFSET + index * SHORTARRAY_SCALE toLong, value)
  	@inline def putInt(array :Array[Int], index :Int, value :Int) = instance.putInt(array, INTARRAY_OFFSET + index * INTARRAY_SCALE toLong, value)
  	@inline def putLong(array :Array[Long], index :Int, value :Long) = instance.putLong(array, LONGARRAY_OFFSET + index * LONGARRAY_SCALE toLong, value)
  	@inline def putFloat(array :Array[Float], index :Int, value :Float) = instance.putFloat(array, FLOATARRAY_OFFSET + index * FLOATARRAY_SCALE toLong, value)
  	@inline def putDouble(array :Array[Double], index :Int, value :Double) = instance.putDouble(array, DOUBLEARRAY_OFFSET + index * DOUBLEARRAY_SCALE toLong, value)
  	@inline def put[A](array :Array[A], index :Int, value :A) = instance.putObject(array, ANYARRAY_OFFSET + index * ANYARRAY_SCALE toLong, value)
}