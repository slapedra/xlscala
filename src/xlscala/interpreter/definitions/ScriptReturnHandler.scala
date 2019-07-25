package xlscala.interpreter.definitions

import aether._
import xlscala.interpreter._
import scala.reflect.runtime.universe._
 
import scala.collection.mutable.ArrayBuffer
 
class ScriptReturnHandler {
	protected var _objs :ArrayBuffer[MetaAny] = ArrayBuffer()	
	
	def objs = _objs
	
	def +=[T](obj :T)(implicit m :TypeTag[T]) = _objs += MetaAny(obj)(m) 
	
	def ++=[T](objs :Iterable[T])(implicit m :TypeTag[T]) {
		for (obj <- objs) _objs += MetaAny(obj)(m) 
	}
	
	def ++=[T1](objs :Tuple1[T1])(implicit m1 :TypeTag[T1]) {
		_objs += MetaAny(objs._1)(m1) 
	}
	
	def ++=[T1, T2](objs :Tuple2[T1, T2])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
	}
	
	def ++=[T1, T2, T3]
	(objs :Tuple3[T1, T2, T3])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
	}
	
	def ++=[T1, T2, T3, T4]
	(objs :Tuple4[T1, T2, T3, T4])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
	}
	
	def ++=[T1, T2, T3, T4, T5]
	(objs :Tuple5[T1, T2, T3, T4, T5])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6]
	(objs :Tuple6[T1, T2, T3, T4, T5, T6])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7]
	(objs :Tuple7[T1, T2, T3, T4, T5, T6, T7])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8]
	(objs :Tuple8[T1, T2, T3, T4, T5, T6, T7, T8])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9]
	(objs :Tuple9[T1, T2, T3, T4, T5, T6, T7, T8, T9])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10]
	(objs :Tuple10[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11]
	(objs :Tuple11[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11)
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12]
	(objs :Tuple12[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13]
	(objs :Tuple13[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14]
	(objs :Tuple14[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15]
	(objs :Tuple15[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16]
	(objs :Tuple16[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17]
	(objs :Tuple17[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16], m17 :TypeTag[T17]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
		_objs += MetaAny(objs._17)(m17) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18]
	(objs :Tuple18[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16], m17 :TypeTag[T17], m18 :TypeTag[T18]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
		_objs += MetaAny(objs._17)(m17) 
		_objs += MetaAny(objs._18)(m18) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19]
	(objs :Tuple19[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16], m17 :TypeTag[T17], m18 :TypeTag[T18], m19 :TypeTag[T19]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
		_objs += MetaAny(objs._17)(m17) 
		_objs += MetaAny(objs._18)(m18) 
		_objs += MetaAny(objs._19)(m19) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20]
	(objs :Tuple20[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16], m17 :TypeTag[T17], m18 :TypeTag[T18], m19 :TypeTag[T19], m20 :TypeTag[T20]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
		_objs += MetaAny(objs._17)(m17) 
		_objs += MetaAny(objs._18)(m18) 
		_objs += MetaAny(objs._19)(m19) 
		_objs += MetaAny(objs._20)(m20) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21]
	(objs :Tuple21[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16], m17 :TypeTag[T17], m18 :TypeTag[T18], m19 :TypeTag[T19], m20 :TypeTag[T20],
			  m21 :TypeTag[T21]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
		_objs += MetaAny(objs._17)(m17) 
		_objs += MetaAny(objs._18)(m18) 
		_objs += MetaAny(objs._19)(m19) 
		_objs += MetaAny(objs._20)(m20) 
		_objs += MetaAny(objs._21)(m21) 
	}
	
	def ++=[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22]
	(objs :Tuple22[T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22])
	(implicit m1 :TypeTag[T1], m2 :TypeTag[T2], m3 :TypeTag[T3], m4 :TypeTag[T4], m5 :TypeTag[T5],
			  m6 :TypeTag[T6], m7 :TypeTag[T7], m8 :TypeTag[T8], m9 :TypeTag[T9], m10 :TypeTag[T10],
			  m11 :TypeTag[T11], m12 :TypeTag[T12], m13 :TypeTag[T13], m14 :TypeTag[T14], m15 :TypeTag[T15],
			  m16 :TypeTag[T16], m17 :TypeTag[T17], m18 :TypeTag[T18], m19 :TypeTag[T19], m20 :TypeTag[T20],
			  m21 :TypeTag[T21], m22 :TypeTag[T22]) {
		_objs += MetaAny(objs._1)(m1) 
		_objs += MetaAny(objs._2)(m2) 
		_objs += MetaAny(objs._3)(m3) 
		_objs += MetaAny(objs._4)(m4) 
		_objs += MetaAny(objs._5)(m5) 
		_objs += MetaAny(objs._6)(m6) 
		_objs += MetaAny(objs._7)(m7) 
		_objs += MetaAny(objs._8)(m8) 
		_objs += MetaAny(objs._9)(m9) 
		_objs += MetaAny(objs._10)(m10) 
		_objs += MetaAny(objs._11)(m11) 
		_objs += MetaAny(objs._12)(m12) 
		_objs += MetaAny(objs._13)(m13) 
		_objs += MetaAny(objs._14)(m14) 
		_objs += MetaAny(objs._15)(m15) 
		_objs += MetaAny(objs._16)(m16)
		_objs += MetaAny(objs._17)(m17) 
		_objs += MetaAny(objs._18)(m18) 
		_objs += MetaAny(objs._19)(m19) 
		_objs += MetaAny(objs._20)(m20) 
		_objs += MetaAny(objs._21)(m21) 
		_objs += MetaAny(objs._22)(m22) 
	}

}