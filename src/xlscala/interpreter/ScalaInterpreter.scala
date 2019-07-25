package xlscala.interpreter

import scala.reflect.runtime.universe._
import scala.tools.nsc._
import scala.tools.nsc.interpreter._
import scala.tools.nsc.reporters.AbstractReporter
import scala.tools.nsc.{Global, Settings}

import scala.collection.mutable.ArrayBuffer
import scala.collection.mutable.ListBuffer
import scala.collection.mutable.Set
import scala.collection.mutable.Map
import scala.collection.mutable.HashMap
import scala.concurrent.Lock

import java.util.StringTokenizer
import java.math.BigInteger
import java.security.MessageDigest


import aether._
import xlscala.interpreter.definitions._

  
trait ObjectNamor {
	def +=(value :Int)
	def ++=(value :Int)
	def id(obj :MetaAny) :String
}

class SimpleObjectNamor extends ObjectNamor {
	protected var _i = 0
	protected var _j = 0
	protected var _index = -1
	
	def +=(value :Int) = _i += 1
	def ++=(value :Int) = _j += 1
	def id(obj :MetaAny) :String = {
		var ntype = obj.tpe.toString.toLowerCase
		var tmp2 = ntype.indexOf('[')
		var nntype = ""
		if (tmp2 != -1) {
			nntype = ntype.substring(0, tmp2)
		} else {
			nntype = ntype
		}
		_index += 1
		return nntype.substring(nntype.lastIndexOf('.')+1) + ":" + _index;
	}
}


final class ScalaInterpreter(imports :Iterable[String] = List(), protected var _classpath :String = null) {
	
	protected var _index = 0L
	
	protected def objectname(name :String) :String = {
		var ret = new java.lang.StringBuilder
		ret.append(name).append(_index).append("_")
		_index += 1
		return ret.toString
	}
	
	protected class Interpreter(initialSettings: Settings, out: JPrintWriter) extends IMain(initialSettings, out) {
		def lastResult = prevRequestList.last.lineRep.call("$result")
	}
	
	//protected val _sha1 = MessageDigest.getInstance("SHA-1")
	
	protected def uniqueId(code: String): String = {
		return code
		//val digest = _sha1.digest(code.getBytes())
		//return new BigInteger(1, digest).toString(16)
	}

	type Context = ScalaInterpreterContext
	
	protected class CircularWriter(ssize :Int) extends java.io.Writer {
		protected var _lines :Array[String] = Array.fill(ssize){""}
		protected var _index = 0
       
		override def write(chars :Array[Char], off :Int, len :Int) {
			if (_index >= _lines.size) {
				_index = 0
			}
			_lines(_index) += new String(chars, off, len-1)
			_index += 1
		}
               
		override def flush {}
		override def close {}
               
		def apply(index :Int) = _lines((index + _index) % _lines.size)
               
		def size = _lines.size
		
		def last = {
			var ret = new java.lang.StringBuilder
			if (_index >= 3) {
				ret append _lines((_index-3) % _lines.size) append "\n" append _lines((_index-2) % _lines.size) append "\n" append _lines((_index-1) % _lines.size)
			} else if (_index == 2){
				ret append _lines((_index-2) % _lines.size) append "\n" append _lines((_index-1) % _lines.size)
			} else if (_index == 1){
				ret append _lines((_index-1) % _lines.size)
			} 
			ret.toString
		}
	}
       
	protected var _settings = new Settings
	if (_classpath != null)_settings.classpath.value += _classpath
	_settings.usejavacp.value = true
	_settings.nowarnings.value = true
	protected var _writer = new CircularWriter(Defines.MAX_CIRCULARLINES)
	protected var _compiler = new Interpreter(_settings, new java.io.PrintWriter(_writer))
        
	protected type EvalFunction = Function1[Iterable[MetaAny], MetaAny]
	protected type ExecFunction = Function1[Iterable[MetaAny], Iterable[MetaAny]]
	protected type FillFunction = Function3[MetaAny, MetaAny, MetaAny, Unit]
	protected type SequencedFillFunction = Function3[MetaAny, MetaAny, Int, Unit]
	protected type SetLikeFillFunction = Function2[MetaAny, MetaAny, Unit]
	protected type GetFunction = Function1[MetaAny, Array[MetaAny]]
	protected var _evals :Map[String, Either[EvalFunction, String]] = HashMap()
	protected var _execs :Map[String, Either[ExecFunction, String]] = HashMap()
	protected var _fills :Map[String, Either[FillFunction, String]] = HashMap()
	protected var _setlikefills :Map[String, Either[SetLikeFillFunction, String]] = HashMap()
	protected var _sequencedfills :Map[String, Either[SequencedFillFunction, String]] = HashMap()
	protected var _gets :Map[String, Either[GetFunction, String]] = HashMap()
	protected var _imports :Map[String, Bool] = HashMap()
	
	protected var _lock = new Lock

	{ 
		var importStr = new java.lang.StringBuilder
		var code = new java.lang.StringBuilder
		for (im <- imports) importStr append "import " append im append "\n"
		require(_compiler.interpret(code append 
				"import scala.collection.mutable.ListBuffer\n" append 
				"import scala.collection.mutable.ArrayBuffer\n" append
				"import scala.collection.mutable.Map\n" append
				"import scala.collection.mutable.HashMap\n" append
				"import scala.collection.mutable.Set\n" append
				"import scala.reflect.runtime.universe._\n" append 
				"import scala.language.experimental.macros\n" append
				"import scala.reflect.macros.Context\n" append
				"import aether._\n" append
				"import xlscala.interpreter._\n" append
				"import xlscala.interpreter.definitions._\n" append
				importStr.toString toString
				) != Results.Error, "error importing initial packages")
	}
	
	def clear = {
		_evals = HashMap()
		_gets = HashMap()
		_fills = HashMap()
		_gets = HashMap()
	}
	
    def use(context :Context, imports :Iterable[String]) {
		for (imp <- imports) {
			_imports.get(imp) match {
				case Some(flag) => require(flag == true, "error importing package " + imp)
				case None => {
					var fimport = new java.lang.StringBuilder append 
					  "object _import_ {\n" append 
					  "\tdef apply {\n" append
					  "\t\timport " append imp append ";\n" append
					  "\t}\n}" toString;
					_lock.acquire
					var res = _compiler.interpret(fimport)
					if (res == Results.Error){
						_imports += imp -> false
						_lock.release
						error("error importing package " + imp)
					} else {
						_imports += imp -> true
						_lock.release
					}
				}
			}
		}
		context.addImports(imports)
	}
	
    def use(context :Context, name :String) {
		use(context, List(name))
	}
	
	protected def metaToCode(meta :MetaAny) :String = {
		if (meta.tpe != null) {
			return new java.lang.StringBuilder append ".asInstanceOf[" append meta.tpe append "]" toString
		} else {
			return ""
		}
	}
	
	protected def _eval(context :Context, code :String, args :Iterable[MetaAny], bindings :Iterable[Any]) :MetaAny = {
		var feval = new java.lang.StringBuilder append
					"object _eval_ extends Function1[Iterable[MetaAny], MetaAny] {\n" append
					"\tdef apply(_args_ :Iterable[MetaAny]) :MetaAny  = {\n" append
					context.importStr append
					"\t\tvar _aiter_ = _args_.iterator\n" 
		require(args.size == bindings.size, "arguments and bindings must have the same size")
		var aiter = args.iterator
		var biter = bindings.iterator
		var index = 1
		while (aiter.hasNext) {
			var arg = aiter.next
			var binding = biter.next
			binding match {
				case null => feval append "\t\tvar _" append index append " = _aiter_.next.any" append metaToCode(arg) append "\n" 	
				case _ => feval append "\t\tvar _" append binding append " = _aiter_.next.any" append metaToCode(arg) append "\n"
			} 
			index += 1 
		}
		feval append "\n\t\t{\n\t\t\treturn MetaAny(" append code append ")\n\t\t}\n\t}\n}"
		var sfeval = feval.toString
		var id = uniqueId(sfeval)
		_evals.get(sfeval) match {
			case Some(of) => {
				of match {
					case Left(f) => return f(args) 
					case Right(msg) => error("scala error (eval): \n" + sfeval + "\n" + msg)
				}
			}
			case None => {
				_lock.acquire
				if (_compiler.interpret(sfeval) == Results.Error) {
					_evals += id -> Right(_writer.last)
					_lock.release
					error("scala error (eval): \n" + sfeval + "\n" + _writer.last)
				}
				var f = _compiler.lastResult.asInstanceOf[EvalFunction]
				_evals += id -> Left(f)
				_lock.release
				return f(args)
			}
		}
	}
	
	protected def eval(context :Context, any :Any, args :Iterable[MetaAny] = List(), bindings :Iterable[Any]) :MetaAny = {
		any match {
			case v :Double => return MetaAny(v)(TypeTag.Double)
			case v :Float => return MetaAny(v)(TypeTag.Float)
			case v :Long => return MetaAny(v)(TypeTag.Long)
			case v :Int => return MetaAny(v)(TypeTag.Int)
			case v :Short => return MetaAny(v)(TypeTag.Short)
			case v :Byte => return MetaAny(v)(TypeTag.Byte)
			case v :Unit => return MetaAny(v)(TypeTag.Unit)
			case v :Boolean => return MetaAny(v)(TypeTag.Boolean)
			case str :String => {
				if (str.size > 0) {
					if (str(0) == Defines.EXPRESSION_TOKEN) {
						var sstr = str.substring(1)
						context.objects.get(sstr) match {
							case Some(obj) => return obj
							case None => return _eval(context, sstr, args, bindings)
						}
					} else return MetaAny(str)(typeTag[String])
				} else return MetaAny(str)(typeTag[String])
			}
			case null => return MetaAny(null)(TypeTag.Null)
			case _ => error("unsupported type: " + any)
		}
	}
    
	protected def _exec(context :Context, codes :Iterable[String], args :Iterable[MetaAny], bindings :Iterable[Any]) :Iterable[MetaAny] = {
    	var fexec = new java.lang.StringBuilder append 
    				"object _exec_ extends Function1[Iterable[MetaAny], Iterable[MetaAny]] {\n" append
					"\tdef apply(_args_ :Iterable[MetaAny]) :Iterable[MetaAny] = {\n" append
					context.importStr append
					"\t\tvar _return = new ScriptReturnHandler\n" append  
					"\t\tvar _aiter_ = _args_.iterator\n"
		require(args.size == bindings.size, "arguments and bindings must have the same size")
		var aiter = args.iterator
		var biter = bindings.iterator
		var index = 1
		while (aiter.hasNext) {
			var arg = aiter.next
			var binding = biter.next
			binding match {
				case null => fexec append "\t\tvar _" append index append " = _aiter_.next.any" append metaToCode(arg) append "\n"	
				case _ => fexec append "\t\tvar _" append binding append " = _aiter_.next.any" append metaToCode(arg) append "\n"
			} 
			index += 1
		}
		fexec append "\n\t\tdef _innerexec_ {\n"
		for (code <- codes) {
			if (code != null) {
				fexec append " \t\t\t" append code append "\n"
			} else {
				fexec append " \t\t\t\n"
			}
		}
		fexec append "\t\t}\n"
		fexec append "\t\t_innerexec_\n\t\treturn _return.objs\n\t}\n}"
		var sfexec = fexec.toString
		var id = uniqueId(sfexec)
		_execs.get(id) match {
			case Some(of) => {
				of match {
					case Left(f) => return f(args)
					case Right(msg) => error("scala error (exec): \n" + sfexec + "\n" + msg)
				}
			}
			case None => {
				_lock.acquire
				if (_compiler.interpret(sfexec) == Results.Error) {
					_execs += id -> Right(_writer.last)
					_lock.release
					error("scala error (exec): \n" + fexec + "\n" + _writer.last)
				}
				var f = _compiler.lastResult.asInstanceOf[ExecFunction]
				_execs += id -> Left(f)
				_lock.release
				return f(args)
			}
		}
	}
    
	protected def _sequencedfill(context :Context, meta :MetaAny, value :MetaAny, index :Int) {
		var ffill = new java.lang.StringBuilder append
					"object _sequencedfill_ extends Function3[MetaAny, MetaAny, Int, Unit] {\n" append
					"\tdef apply(_meta_ :MetaAny, _value_ :MetaAny, _index_ :Int) :Unit = {\n" append
					context.importStr append
					"\t\tvar _object_ = _meta_.any" append metaToCode(meta) append "\n"
		ffill append "\t\t_object_(_index_)  = _value_.any" append metaToCode(value) append "\n"
		ffill append "\t}\n}"
		var sffill = ffill.toString
		var id = uniqueId(sffill)
		_sequencedfills.get(id) match {
			case Some(of) => {
				of match {
					case Left(f) => f(meta, value, index)
					case Right(msg) => error("scala error (ffil): \n" + sffill + "\n" + msg)
				}
			}
			case None => {
				_lock.acquire
				if (_compiler.interpret(sffill) == Results.Error) {
					_sequencedfills += id -> Right(_writer.last)
					_lock.release
					error("scala error (fill): \n" + ffill + "\n" + _writer.last)
				}
				var f = _compiler.lastResult.asInstanceOf[SequencedFillFunction]
				_sequencedfills += id -> Left(f)
				_lock.release
				f(meta, value, index)
			}
		}
	}
	
	protected def _fill(context :Context, meta :MetaAny, value :MetaAny, index :MetaAny) {
		var ffill = new java.lang.StringBuilder append
					"object _fill_ extends Function3[MetaAny, MetaAny, MetaAny, Unit] {\n" append
					"\tdef apply(_meta_ :MetaAny, _value_ :MetaAny, _index_ :MetaAny) :Unit = {\n" append
					context.importStr append
					"\t\tvar _object_ = _meta_.any" append metaToCode(meta) append "\n"
		ffill append "\t_object_(_index_.any" append metaToCode(index) append ") = _value_.any" append metaToCode(value) append "\n"
		ffill append "\t}\n}"
		var sffill = ffill.toString
		var id = uniqueId(sffill)
		_fills.get(id) match {
			case Some(of) => {
				of match {
					case Left(f) => f(meta, value, index)
					case Right(msg) => error("scala error (ffil): \n" + sffill + "\n" + msg)
				}
			}
			case None => {
				_lock.acquire
				if (_compiler.interpret(sffill) == Results.Error) {
					_fills += id -> Right(_writer.last)
					_lock.release
					error("scala error (fill): \n" + ffill + "\n" + _writer.last)
				}
				var f = _compiler.lastResult.asInstanceOf[FillFunction]
				_fills += id -> Left(f)
				_lock.release
				f(meta, value, index)
			}
		}
	}
	
	protected def _setlikefill(context :Context, meta :MetaAny, value :MetaAny) {
		var ffill = new java.lang.StringBuilder append 
					"object _fill_ extends Function2[MetaAny, MetaAny, Unit] {\n" append
					"\tdef apply(_meta_ :MetaAny, _value_ :MetaAny) :Unit = {\n" append
					context.importStr append
					"\t\tvar _object_ = _meta_.any" append metaToCode(meta) append "\n"
		ffill append "\t_object_ += _value_.any" append metaToCode(value) append "\n"
		ffill append "\t}\n}"
		var sffill = ffill.toString
		var id = uniqueId(sffill)
		_setlikefills.get(id) match {
			case Some(of) => {
				of match {
					case Left(f) => f(meta, value)
					case Right(msg) => error("scala error (ffil): \n" + sffill + "\n" + msg)
				}
			}
			case None => {
				_lock.acquire
				if (_compiler.interpret(sffill) == Results.Error) {
					_setlikefills += id -> Right(_writer.last)
					_lock.release
					error("scala error (fill): \n" + ffill + "\n" + _writer.last)
				}
				var f = _compiler.lastResult.asInstanceOf[SetLikeFillFunction]
				_setlikefills += id -> Left(f)
				_lock.release
				f(meta, value)
			}
		}
	}
	
	protected def fill(context :Context, meta :MetaAny, values :Iterable[MetaAny], indexes :Iterable[MetaAny]) {
		require(values.size == indexes.size, "values and indexes must have the same size")
		var viter = values.iterator 
		var iiter = indexes.iterator
		if (meta.tpe <:< typeOf[scala.collection.mutable.SetLike[_, Set[_]]]) {
			while (viter.hasNext) {
				var value = viter.next
				_setlikefill(context, meta, value)
			}
		} else {
			var index = 0
			while (viter.hasNext) {
				var value = viter.next
				var iindex = iiter.next
				if (iindex.any == null) {
					_sequencedfill(context, meta, value, index)
				} else {
					_fill(context, meta, value, iindex)
				}
				index += 1
			}
		}
	}
	
	protected def getIterable(meta :MetaAny) :Array[MetaAny] = {
		var fget = new java.lang.StringBuilder append
					"object _get_ extends Function1[MetaAny, Array[MetaAny]] {\n" append
				    "\tdef apply(_meta_ :MetaAny) :Array[MetaAny] = {\n" append
					"\t\tvar _object_ = _meta_.any" append metaToCode(meta) append "\n" append
					"\t\tvar _result_ = new Array[MetaAny](_object_.size)\n" append
					"\t\tvar _index_ = 0\n" append
					"\t\tfor (x <- _object_) {\n" append
					"\t\t\t_result_(_index_) = MetaAny(x)\n" append
					"\t\t\t_index_ += 1\n" append
					"\t\t}\n" append
					"\t\treturn _result_\n" append 
					"\t}\n}"
		var sfget = fget.toString
		var id = uniqueId(sfget)
		_gets.get(id) match {
			case Some(of) => {
				of match {
					case Left(f) => f(meta)
					case Right(msg) => error("scala error (get): \n" + sfget + "\n" + msg)
				}
			}
			case None => {
				_lock.acquire
				if (_compiler.interpret(sfget) == Results.Error) {
					_fills += id -> Right(_writer.last)
					_lock.release
					error("scala error (get): \n" + sfget + "\n" + _writer.last)
				}
				var f = _compiler.lastResult.asInstanceOf[GetFunction]
				_gets += id -> Left(f)
				_lock.release
				return f(meta)
			}
		}
	}
	
	protected def _bind(context :Context, name :String, meta :MetaAny, force :Bool = false) :Any = {
		meta.any match {
			case v :Double => return v
			case v :Float => return v
			case v :Long => return v
			case v :Int => return v
			case v :Short => return v
			case v :Byte => return v
			case v :Unit => return v
			case v :Boolean => return v
			case str :String => return str 
			case null => return null
			case c :chronos.Date => return c.serial
			case _ => {
				context.addObject(name -> meta)
				return Defines.STRING_EXPRESSION_TOKEN + name
			}
		}
	} 
	
	protected def tupleBind(context :Context, namor :ObjectNamor, obj :MetaAny, force :Bool = false) :Array[Any] = {
		var index = 0
		var miter =  typeArgs(obj.tpe).iterator
		var piter =  obj.any.asInstanceOf[Product].productIterator
		var ret = new Array[Any](piter.size)
		for (value <- obj.any.asInstanceOf[Product].productIterator) {
			var tpe = miter.next
			var obj = new MetaAny(value, tpe)
			ret(index) = _bind(context, namor.id(obj), obj, force)
			namor += 1
			index += 1
		}
		return ret
	}

    @inline
	protected def _set(context :Context,
					   scalar :Any, 
					   aargs :Iterable[Any], 
					   bbindings :Iterable[Any],
					   vvalues :Iterable[Any], 
					   iindexes :Iterable[Any]) :MetaAny = {
		require(aargs.size == bbindings.size, "arguments and bindings must have the same size")
		var args :ArrayBuffer[MetaAny] = ArrayBuffer()
		var bindings :ArrayBuffer[Any] = ArrayBuffer()
		var biter = bbindings.iterator
		for (arg <- aargs) {
			args += eval(context, arg, args, bindings)
			bindings += biter.next
		}
		var values :ArrayBuffer[MetaAny] = ArrayBuffer()
		for (value <- vvalues) values += eval(context, value, args, bindings)
		var indexes :ArrayBuffer[MetaAny] = ArrayBuffer()
		var n = MetaAny(null)(TypeTag.Null)
		if (iindexes.size >0) {
			require(vvalues.size == iindexes.size, "indexes and values must have the same size")
			for (iindex <- iindexes) {
				if (iindex != null) {
					indexes += eval(context, iindex, args, bindings)
				} else {
					indexes += n
				}
			}
		} else {
			for (value <- vvalues) indexes += n
		}
		var obj = eval(context, scalar, args, bindings)
		if (values.size != 0) fill(context, obj, values, indexes)
		return obj
	}
	
	def set(context :Context,
			namor :ObjectNamor, scalar :Any, 
			args :Iterable[Any], 
			bindings :Iterable[Any],
			values :Iterable[Any] = List(), 
			indexes :Iterable[Any] = List()) :Any = {
		var obj = _set(context, scalar, args, bindings, values, indexes)
		return _bind(context, namor.id(obj), obj)
	}
	
	protected def _attrs(context :Context, 
						 scalar :Any, attr :String,
					     aargs :Iterable[Any], 
					     bbindings :Iterable[Any]) :MetaAny = {
		require(aargs.size == bbindings.size, "arguments and bindings must have the same size")
		var args :ArrayBuffer[MetaAny] = ArrayBuffer()
		var bindings :ArrayBuffer[Any] = ArrayBuffer()
		var biter = bbindings.iterator
		for (arg <- aargs) {
			args += eval(context, arg, args, bindings)
			bindings += biter.next
		}
		var obj = eval(context, scalar, args, bindings)
		args += obj
		bindings += "objwithattr_"
		if (aargs.size == 0) {
			if ( (attr.size == 0) || (attr == null) ) {
				return eval(context, "__objwithattr_()", args, bindings)
			} else {
				return eval(context, "__objwithattr_." + attr, args, bindings)
			}
		} else {
			if ( (attr.size == 0) || (attr == null) ) {
				var code = new java.lang.StringBuilder append "__objwithattr_.apply(_1"
				for (index <- 2 to aargs.size) code append ", _"  append index
				code append ")"
				return eval(context, code.toString, args, bindings)
			} else {
				var code = new java.lang.StringBuilder append "__objwithattr_." append attr append "(_1"
				for (index <- 2 to aargs.size) code append ", _" append index
				code append ")"
				return eval(context, code.toString, args, bindings)
			}
		}
	}
	
	def attr(context :Context, 
			 namor :ObjectNamor, scalar :Any, attr :String, 
			 args :Iterable[Any], 
			 bindings :Iterable[Any]) :Any = {
		var obj = _attrs(context, scalar, attr, args, bindings)
		return _bind(context, namor.id(obj), obj)
	}
	
	def exec(context :Context,
			 namor :ObjectNamor, 
			 codes :Iterable[String], 
			 aargs :Iterable[Any], 
			 bbindings :Iterable[Any]) :Array[Any] = {
		require(aargs.size == bbindings.size, "arguments and bindings must have the same size")
		var args :ArrayBuffer[MetaAny] = ArrayBuffer()
		var bindings :ArrayBuffer[Any] = ArrayBuffer()
		var biter = bbindings.iterator
		for (arg <- aargs) {
			args += eval(context, arg, args, bindings)
			bindings += biter.next
		}
		var objs = _exec(context, codes, args, bindings)
		var ret :Array[Any] = new Array[Any](objs.size)
		var index = 0
		for (obj <- objs) {
			ret(index) = _bind(context, namor.id(obj), obj)
			namor += 1
			index += 1
		}
		return ret
	}
	
	def exec(context :Context, namor :ObjectNamor, code :String, aargs :Iterable[Any], bbindings :Iterable[Any]) :Array[Any] = exec(context, namor, Array(code), aargs, bbindings)
	
	def getType(context :Context, scalar :Any) :String = {
		return eval(context, scalar, List(), List()).tpe.toString
	}
	
	protected def _get(context :Context, namor :ObjectNamor, obj :MetaAny, force :Bool) :Pair[Array[Any], Array[Int]] = {
		obj.any match {
			case t :Tuple1[_] => return (tupleBind(context, namor, obj, force), Array(1))
			case t :Tuple2[_, _] => return (tupleBind(context, namor, obj, force), Array(2))
			case t :Tuple3[_, _, _ ] => return (tupleBind(context, namor, obj, force), Array(3))
			case t :Tuple4[_, _, _, _ ] => return (tupleBind(context, namor, obj, force), Array(4))
			case t :Tuple5[_, _, _, _, _ ] => return (tupleBind(context, namor, obj, force), Array(5))
			case t :Tuple6[_, _, _, _, _, _ ] => return (tupleBind(context, namor, obj, force), Array(6))
			case t :Tuple7[_, _, _, _, _, _, _ ] => return (tupleBind(context, namor, obj, force), Array(7))
			case t :Tuple8[_, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(8))
			case t :Tuple9[_, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(9))
			case t :Tuple10[_, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(10))
			case t :Tuple11[_, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(11))
			case t :Tuple12[_, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(12))
			case t :Tuple13[_, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(13))
			case t :Tuple14[_, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(14))
			case t :Tuple15[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(15))
			case t :Tuple16[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(16))
			case t :Tuple17[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(17))
			case t :Tuple18[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(18))
			case t :Tuple19[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(19))
			case t :Tuple20[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(20))
			case t :Tuple21[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(21))
			case t :Tuple22[_, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _, _] => return (tupleBind(context, namor, obj, force), Array(22))
			case t :Array[_] => {
				var metas = getIterable(obj)
				var ret = new Array[Any](metas.size)
				var index = 0
				for (meta <- metas) {
					ret(index) = _bind(context, namor.id(meta), meta, force)
					namor += 1
					index += 1
				}
				return (ret, Array(metas.size))
			}
			case t :Iterable2[_] => {
				var metas = getIterable(obj)
				var ret = new Array[Any](metas.size)
				var index = 0
				for (meta <- metas) {
					ret(index) = _bind(context, namor.id(meta), meta, force)
					namor += 1
					index += 1
				}
				return (ret, Array(t.size1, t.size2))
			}
			case t :Iterable[_] => {
				var metas = getIterable(obj)
				var ret = new Array[Any](metas.size)
				var index = 0
				for (meta <- metas) {
					ret(index) = _bind(context, namor.id(meta), meta, force)
					namor += 1
					index += 1
				}
				return (ret, Array(metas.size))
			}
			case _ => {
				return (Array(_bind(context, namor.id(obj), obj, force)), Array(1))
			}
		}
	}
	
	def get(context :Context, namor :ObjectNamor, scalar :Any, 
			args :Iterable[Any], 
			bindings :Iterable[Any], 
			force :Bool) :Pair[Array[Any], Array[Int]] = {
		var obj = _set(context, scalar, args, bindings, List(), List())
		return _get(context, namor, obj, force)
	}
	
	def getAttr(context :Context, namor :ObjectNamor, scalar :Any, attr :String, 
			args :Iterable[Any], 
			bindings :Iterable[Any], 
			force :Bool) :Pair[Array[Any], Array[Int]] = {
		var obj = _attrs(context, scalar, attr, args, bindings)
		return _get(context, namor, obj, force)
	}

	def listAttr(context :Context, scalar :Any) :ArrayBuffer[String] = {
		var ret :ArrayBuffer[String] = ArrayBuffer()
		var obj = eval(context, scalar, List(), List())
		obj.any match {
			case ref :AnyRef => {
				var methods = ref.getClass.getMethods
				for (method <- methods) ret += method.toString
			}
			case _ =>
		}
		return ret
	}
}



object ScalaInterpreter {
	def apply(imports :Iterable[String] = List(), classpath :String = null) = new ScalaInterpreter(imports, classpath)	
	

}

