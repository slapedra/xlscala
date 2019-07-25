package xlscala

import aether._
import xlscala.interpreter._

import scala.collection.mutable.Map
import scala.collection.mutable.HashMap

final class ExcelNamorMap {
	protected var _id :Int = 0
	protected var _map :Map[Tuple3[Int, Int, Int], Int] = HashMap()
	
	def id(page :Int, row :Int, col :Int) :Int = {
		_map.get((page, row, col)) match {
			case Some(id) =>{
				return id
			}
			case None => {
				_map += (page, row, col) -> _id
				_id += 1
				return _id - 1
			}
		}
	}
}

final class ExcelNamor(protected var _map :ExcelNamorMap,
				 	   var page :Int, var row :Int, var col :Int, 
				 	   var transpose :Boolean = false)  
extends ObjectNamor {  

	protected val BASE_SHIFT			= 7
	protected val BASE_MASK   			= scala.math.pow(2, BASE_SHIFT).toInt - 1
	protected val BASE_ALPHABET			= "0123456789" +
										  "abcdefghijklmnopqrstuvwxyz" + 
										  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" +
										  "£§" + 
										  "ÇÉáÓüæíĞßéÆóÊÔâôúËÒäöñÈõ¶àòÑÁÕåç" + 
										  "ûÂãÍµùÀÃÎşêÿÏŞëÖÚèÜÛïÙîıìØİÄ¥ÌÅ©" 
	
	def +=(value :Int) = if (transpose) col += value else row += value
	def ++=(value :Int) = if (transpose) row += value else col += value
               
	def id(obj :MetaAny) :String = {
		var iid = _map.id(page, row, col)
		require(iid < (2 << 21), "maximum number of objects reached")
		var array :Array[Char] = new Array(3)
		var tmp = iid
		array(2) = BASE_ALPHABET((tmp & BASE_MASK).toInt)
		tmp = tmp >> BASE_SHIFT
		array(1) = BASE_ALPHABET((tmp & BASE_MASK).toInt)
		tmp = tmp >> BASE_SHIFT
		array(0) = BASE_ALPHABET((tmp & BASE_MASK).toInt)
		var ret = new String(array)
		var ntype = obj.tpe.toString.toLowerCase
		var tmp2 = ntype.indexOf('[')
		var nntype = ""
		if (tmp2 != -1) {
			nntype = ntype.substring(0, tmp2)
		} else {
			nntype = ntype
		}
		return nntype.substring(nntype.lastIndexOf('.')+1) + ":" + ret;
	}
}