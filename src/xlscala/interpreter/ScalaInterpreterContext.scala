package xlscala.interpreter

import aether._
import xlscala.interpreter.definitions._
import scala.reflect.runtime.universe._

final class ScalaInterpreterContext {
	 
	protected var _importStr :String = ""
	protected var _imports :Set[String] = Set()
		
	protected var _objects :Map[String, MetaAny] = Map()
	
	def importStr = _importStr
	def objects = _objects
	 
	def clear {
		_importStr = ""
		_imports = Set()
		_objects = Map()
	}
	
	def addObject(p :Pair[String, MetaAny]) = _objects += p
	

	def addImports(imports :Iterable[String]) {
		_imports ++= imports
		var importStr = new java.lang.StringBuilder
		for (p <- _imports) {
			importStr append "\t\timport " append p append "\n"	
		}
		_importStr = importStr.toString
	}
	
	@inline
	protected def metaToCode(meta :MetaAny) :String = {
		if (meta.tpe != null) {
			return new java.lang.StringBuilder append ".asInstanceOf[" append meta.tpe append "]" toString
		} else {
			return ""
		}
	}
}