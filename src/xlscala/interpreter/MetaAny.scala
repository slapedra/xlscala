package xlscala.interpreter

import scala.reflect.runtime.universe._

class MetaAny(protected var _obj :Any, protected var _tpe :Type){
	def obj = _obj
	def any = _obj
	def tpe = _tpe
	
	def :=[Type](obj :Type)(implicit t :TypeTag[Type]) {
		_obj = obj
		_tpe = t.tpe
	}
}

object MetaAny {
	def apply[@specialized Type](obj :Type)(implicit t :TypeTag[Type]) = new MetaAny(obj, t.tpe)	
	
}