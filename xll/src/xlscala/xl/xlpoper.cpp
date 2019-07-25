#include <xlscala/xl/xlpoper.hpp>
#include <xlscala/xl/xloper.hpp>
#include <xlscala/xl/scalaropervisitor.hpp>

namespace xlscala {

	std::string toString(const std::string& str) {
	    return str;
	}

	XlpOper::XlpOper(OPER *xIn, const std::string& id, bool transpose,
		bool forceScalar, bool reduce)
	: oper_(xIn), id_(id), flagToFree_(false), scalars_(new Range<Scalar>()) {
		try {
			if (!reduce) {
			    validateRange();
				if (!(forceScalar) && oper_->xltype & xltypeMulti) {
					forceScalar_ = false;
				} else {
					forceScalar_ = true;
				}
				convertToScalarVector(transpose);
			} else {
				if (!(forceScalar) && oper_->xltype & xltypeMulti) {
					forceScalar_ = false;
				} else {
					forceScalar_ = true;
				}
				reduceToScalarVector(transpose);

			}
		} catch (Error& e) {
			throw e;
		}
	}

	XlpOper::XlpOper(OPER *xIn, const std::string& id, const Scalar& dflt,
                      bool transpose, bool forceScalar)
	: oper_(xIn), id_(id), flagToFree_(false), scalars_(new Range<Scalar>()) {
		try {
            if (!(forceScalar) && oper_->xltype & xltypeMulti) {
                forceScalar_ = false;
            } else {
                forceScalar_ = true;
            }
            convertToScalarVector(transpose, dflt);
		} catch (Error& e) {
			throw e;
		}
	}

	XlpOper::XlpOper(const Integer& i, const std::string& id, bool transpose, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = i;
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const Real& r, const std::string& id, bool transpose, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = r;
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const std::string& s , const std::string& id, bool transpose, bool dllfree)
	:  id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = s;
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const char *c, const std::string& id, bool transpose, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = std::string(c);
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const bool& b, const std::string& id, bool transpose, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = b;
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const Null& n, const std::string& id, bool transpose, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = Scalar();
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const Scalar& s, const std::string& id, bool transpose, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(new Range<Scalar>(1, 1)) {
		try {
			oper_ = new OPER;
			scalars_->data[0] = s;
			xlscala::apply_visitor(ScalarToOperVisitor(oper_, dllfree), scalars_->data[0]);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::XlpOper(const std::shared_ptr< Range<Scalar> >& sc, const std::string& id,
		bool transpose, bool pretty, bool dllfree)
	: id_(id), forceScalar_(true), flagToFree_(false), scalars_(sc) {
		try {
			oper_ = new OPER;
			convertToOperVector(transpose, pretty, dllfree);
		} catch (Error& e) {
			freeOper(oper_);
			throw e;
		}
	}

	XlpOper::~XlpOper() {
		if (flagToFree_) {
			free(oper_);
		}
	}

	bool XlpOper::validateMulti() {
		for (int i=0; i<oper_->val.array.rows * oper_->val.array.columns; ++i)
			if (oper_->val.array.lparray[i].xltype & xltypeErr)
				return false;
		return true;
	}

	void XlpOper::validateRange() {
		REQUIRE(!(oper_->xltype & xltypeErr), "Parameter '" << id_ << "' has error value");
		REQUIRE(!(oper_->xltype & xltypeMulti) || validateMulti(),
			"Parameter '" << id_ << "' has error value");
	}

	bool XlpOper::missing() const {
		return XlpOper::missing(oper_);
	}

	bool XlpOper::missing(const OPER *oper) {
		return (oper->xltype & xltypeNil
        ||  oper->xltype & xltypeMissing
        ||  (oper->xltype & xltypeErr && oper->val.err == xlerrNA ));
	}

	bool XlpOper::error(const OPER *oper) {
		return (oper->xltype & xltypeErr && oper->val.err != xlerrNA);
	}

	bool XlpOper::scalar(const OPER *oper) {
		return (oper->xltype & xltypeNum
			|| oper->xltype & xltypeBool
			|| oper->xltype & xltypeStr);
	}

	Scalar XlpOper::convertNonMultiOperToScalar(const OPER *oper) {
		if (oper->xltype & xltypeNum)
			return oper->val.num;
		else if (oper->xltype & xltypeBool)
      			return (oper->val.xbool != 0);
	    	else if (oper->xltype & xltypeStr)
			return strConv(oper);
		else if (missing(oper))
			return Null();
		else
			FAIL ("unexpected datatype: " << oper->xltype);
	}

	Scalar XlpOper::convertNonMultiOperToScalar(const OPER *oper, const Scalar& dflt) {
		if (oper->xltype & xltypeNum)
			return oper->val.num;
		else if (oper->xltype & xltypeBool)
      			return (oper->val.xbool != 0);
	    	else if (oper->xltype & xltypeStr)
			return strConv(oper);
		else return dflt;
	}

	void XlpOper::convertToScalarVector(bool transpose) {
		static XLOPER12 xlInt;
		try {
			if (missing(oper_) || error(oper_)) {
				scalars_->rows = 0;
				scalars_->columns = 0;
				return;
			}
			if (scalar(oper_)) {
				scalars_->rows = 1;
				scalars_->columns = 1;
				scalars_->data.push_back(convertNonMultiOperToScalar(oper_));
				return;
			}
			const OPER *xMulti;
			Xloper xCoerce;
			if (oper_->xltype & xltypeMulti) {
				xMulti = oper_;
			} else {
				xlInt.xltype = xltypeInt;
				xlInt.val.w = xltypeMulti;
				Excel12(xlCoerce, &xCoerce, 2, oper_, &xlInt);
				xMulti = &xCoerce;
			}
			if (!transpose) {
				scalars_->rows = xMulti->val.array.rows;
				scalars_->columns = xMulti->val.array.columns;
				scalars_->data.reserve(xMulti->val.array.rows * xMulti->val.array.columns);
				for (int i=0; i<xMulti->val.array.rows * xMulti->val.array.columns; ++i) {
					scalars_->data.push_back((convertNonMultiOperToScalar(&xMulti->val.array.lparray[i])));
				}
			} else {
				scalars_->columns = xMulti->val.array.rows;
				scalars_->rows = xMulti->val.array.columns;
				scalars_->data.reserve(xMulti->val.array.rows * xMulti->val.array.columns);
				Size rows = xMulti->val.array.rows;
				Size columns = xMulti->val.array.columns;
				for (Size i=0; i<columns;i++) {
					for (Size j=0; j<rows;j++) {
						scalars_->data.push_back((convertNonMultiOperToScalar(
							&xMulti->val.array.lparray[i+j*columns])));
					}
				}
			}
		} catch (const std::exception &e) {
			FAIL("error converting parameter '" << id_
				<< "' to type scalar vector: " << e.what());
		}
	}

    void XlpOper::convertToScalarVector(bool transpose, const Scalar& dflt) {
		static XLOPER12 xlInt;
		try {
			if (missing(oper_) || error(oper_)) {
				scalars_->rows = 0;
				scalars_->columns = 0;
				return;
			}
			if (scalar(oper_)) {
				scalars_->rows = 1;
				scalars_->columns = 1;
				scalars_->data.push_back(convertNonMultiOperToScalar(oper_, dflt));
				return;
			}
			const OPER *xMulti;
			Xloper xCoerce;
			if (oper_->xltype & xltypeMulti) {
				xMulti = oper_;
			} else {
				xlInt.xltype = xltypeInt;
				xlInt.val.w = xltypeMulti;
				Excel12(xlCoerce, &xCoerce, 2, oper_, &xlInt);
				xMulti = &xCoerce;
			}
			if (!transpose) {
				scalars_->rows = xMulti->val.array.rows;
				scalars_->columns = xMulti->val.array.columns;
				scalars_->data.reserve(xMulti->val.array.rows * xMulti->val.array.columns);
				for (int i=0; i<xMulti->val.array.rows * xMulti->val.array.columns; ++i) {
					scalars_->data.push_back((convertNonMultiOperToScalar(&xMulti->val.array.lparray[i], dflt)));
				}
			} else {
				scalars_->columns = xMulti->val.array.rows;
				scalars_->rows = xMulti->val.array.columns;
				scalars_->data.reserve(xMulti->val.array.rows * xMulti->val.array.columns);
				Size rows = xMulti->val.array.rows;
				Size columns = xMulti->val.array.columns;
				for (Size i=0; i<columns;i++) {
					for (Size j=0; j<rows;j++) {
						scalars_->data.push_back((
                                convertNonMultiOperToScalar(
							&xMulti->val.array.lparray[i+j*columns], dflt)));
					}
				}
			}
		} catch (const std::exception &e) {
			FAIL("error converting parameter '" << id_
				<< "' to type scalar vector: " << e.what());
		}
	}

	void XlpOper::reduceToScalarVector(bool transpose) {
		static XLOPER12 xlInt;
		try {
			if (missing(oper_) || error(oper_)) {
				scalars_->rows = 0;
				scalars_->columns = 0;
				return;
			}
			if (scalar(oper_)) {
				scalars_->rows = 1;
				scalars_->columns = 1;
				scalars_->data.push_back(convertNonMultiOperToScalar(oper_));
				return;
			}
			const OPER *xMulti;
			Xloper xCoerce;
			if (oper_->xltype & xltypeMulti) {
				xMulti = oper_;
			} else {
				xlInt.xltype = xltypeInt;
				xlInt.val.w = xltypeMulti;
				Excel12(xlCoerce, &xCoerce, 2, oper_, &xlInt);
				xMulti = &xCoerce;
			}
			std::vector<Real> countsX;
			std::vector<Real> countsY;
			if (!transpose) {
				scalars_->rows = xMulti->val.array.rows;
				scalars_->columns = xMulti->val.array.columns;
				scalars_->data.reserve(xMulti->val.array.rows * xMulti->val.array.columns);
				countsX.resize(scalars_->rows, 0);
				countsY.resize(scalars_->columns, 0);
				Size rows = xMulti->val.array.rows;
				Size columns = xMulti->val.array.columns;
				for (Size j=0; j<rows;j++) {
					for (Size i=0; i<columns;i++) {
						if ( !missing(&xMulti->val.array.lparray[i+j*columns])
							&& !error(&xMulti->val.array.lparray[i+j*columns])) {
							scalars_->data.push_back((convertNonMultiOperToScalar(
								&xMulti->val.array.lparray[i+j*columns])));
						} else {
							++countsX[j];
							++countsY[i];
						}
					}
				}
			} else {
				scalars_->columns = xMulti->val.array.rows;
				scalars_->rows = xMulti->val.array.columns;
				scalars_->data.reserve(xMulti->val.array.rows * xMulti->val.array.columns);
				countsX.resize(scalars_->rows, 0);
				countsY.resize(scalars_->columns, 0);
				Size rows = xMulti->val.array.rows;
				Size columns = xMulti->val.array.columns;
				for (Size i=0; i<columns;i++) {
					for (Size j=0; j<rows;j++) {
						if ( !missing(&xMulti->val.array.lparray[i+j*columns])
							&& !error(&xMulti->val.array.lparray[i+j*columns])) {
							scalars_->data.push_back((convertNonMultiOperToScalar(
								&xMulti->val.array.lparray[i+j*columns])));
						} else {
							++countsX[i];
							++countsY[j];
						}
					}
				}
			}
			Size delRows = 0;
			Size delCols = 0;
			for (Size i=0;i<countsX.size();++i) {
				if (countsX[i] == scalars_->columns) {
					++delRows;
				}
			}
			for (Size i=0;i<countsY.size();++i) {
				if (countsY[i] == scalars_->rows) {
					++delCols;
				}
			}
			scalars_->rows -= delRows;
			scalars_->columns -= delCols;
			if (scalars_->size() != scalars_->data.size()) {
				FAIL ("cannot reduce matrix");
			}
		} catch (const std::exception &e) {
			FAIL("error converting parameter '" << id_
				<< "' to type scalar vector: " << e.what());
		}
	}

	void XlpOper::convertToOperVector(bool transpose, bool pretty, bool dllfree) {
		Scalar na = Error();
		try {
			oper_->xltype = xltypeMulti;
			oper_->xltype |= xlbitDLLFree;
			Size size = scalars_->size();
			if (size == 0) {
				oper_->xltype = xltypeMissing;
			// scalar
			} else {
				if ( (scalars_->rows == 1) && (scalars_->columns == 1) && pretty ) {
					oper_->val.array.lparray = new OPER[4];
					oper_->val.array.rows = 2;
					oper_->val.array.columns = 2;
					xlscala::apply_visitor(
						ScalarToOperVisitor(&oper_->val.array.lparray[0], dllfree), scalars_->data[0]);
					xlscala::apply_visitor(
						ScalarToOperVisitor(&oper_->val.array.lparray[1], dllfree), na);
                    xlscala::apply_visitor(
						ScalarToOperVisitor(&oper_->val.array.lparray[2], dllfree), na);
					xlscala::apply_visitor(
						ScalarToOperVisitor(&oper_->val.array.lparray[3], dllfree), na);

				// rows == 1
				} else if ( (scalars_->rows == 1) && pretty ) {
					oper_->val.array.lparray = new OPER[scalars_->columns*2];
					if (!transpose) {
						oper_->val.array.rows = 2;
						oper_->val.array.columns = scalars_->columns;
						Size offset = 0;
						for (int j=0;j<oper_->val.array.columns;j++) {
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j], dllfree),
									scalars_->data[offset]);
							offset++;
						}
						for (int j=0;j<oper_->val.array.columns;j++) {
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j+oper_->val.array.columns], dllfree), na);
						}
					} else {
						oper_->val.array.rows = scalars_->columns;
						oper_->val.array.columns = 2;
						Size offset = 0;
						for (int j=0; j<oper_->val.array.rows;j++) {
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j*2], dllfree),
									scalars_->data[offset]);
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j*2+1], dllfree), na);
							offset++;
						}
					}
				// columns == 1
				} else if ( (scalars_->columns == 1) && pretty ) {
					oper_->val.array.lparray = new OPER[scalars_->rows*2];
					if (!transpose) {
						oper_->val.array.rows = scalars_->rows;
						oper_->val.array.columns = 2;
						Size offset = 0;
						for (int j=0; j<oper_->val.array.rows;j++) {
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j*2], dllfree),
									scalars_->data[offset]);
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j*2+1], dllfree), na);
							offset++;
						}

					} else {
						oper_->val.array.rows = 2;
						oper_->val.array.columns = scalars_->rows;
						Size offset = 0;
						for (int j=0;j<oper_->val.array.columns;j++) {
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j], dllfree),
									scalars_->data[offset]);
							offset++;
						}
						for (int j=0;j<oper_->val.array.columns;j++) {
							xlscala::apply_visitor(
								ScalarToOperVisitor(&oper_->val.array.lparray[j+oper_->val.array.columns], dllfree),
                                na);
						}
					}
				// default
				} else {
					oper_->val.array.lparray = new OPER[size];
					if (!transpose) {
						oper_->val.array.rows = scalars_->rows;
						oper_->val.array.columns = scalars_->columns;
						Size offset = 0;
						for (int i=0; i<oper_->val.array.rows;i++) {
							for (int j=0; j<oper_->val.array.columns;j++) {
								xlscala::apply_visitor(
									ScalarToOperVisitor(&oper_->val.array.lparray[j+i*oper_->val.array.columns], dllfree),
										scalars_->data[offset]);
								offset++;
							}
						}
					} else {
						oper_->val.array.rows = scalars_->columns;
						oper_->val.array.columns = scalars_->rows;
						Size offset = 0;
						for (int i=0; i<oper_->val.array.columns;i++) {
							for (int j=0; j<oper_->val.array.rows;j++) {
								xlscala::apply_visitor(
									ScalarToOperVisitor(&oper_->val.array.lparray[i+j*oper_->val.array.columns], dllfree),
										scalars_->data[offset]);
								offset++;
							}
						}
					}
				}
			}
		} catch (const std::exception &e) {
			FAIL("error converting parameter '" << id_
				<< "' from scalar vector to oper: " << e.what());
		}
	}

	XlpOper::operator Integer() const {
		if (missing(oper_))
			FAIL ("missing oper " << id_);
		if (forceScalar_) {
		    try {
                return scalars_->data[0].get<xlscala::Integer>();
		    } catch(Error& e) {
                return scalars_->data[0].get<xlscala::Real>();
            }
		} else {
			FAIL ("vector oper not scalar: " << id_);
		}
	}

	XlpOper::operator Real() const {
		if (missing(oper_)) {
			FAIL ("missing oper " << id_);
		}
		if (forceScalar_) {
			return scalars_->data[0].get<Real>();
		} else {
			FAIL ("vector oper not scalar: " << id_);
		}
	}

	XlpOper::operator bool() const {
		if (missing(oper_))
			FAIL ("missing  oper " << id_);
		if (forceScalar_) {
			return scalars_->data[0].get<bool>();
		} else {
			FAIL ("vector oper not scalar: " << id_);
		}
	}

	XlpOper::operator std::string() const {
		if (missing(oper_))
			return "";
		if (forceScalar_) {
			std::string tmp = scalars_->data[0].get<std::string>();
			return toString(tmp);
		} else {
			FAIL ("vector oper not scalar: " << id_);
		}
	}

	Scalar XlpOper::scalar() const {
		if (missing(oper_))
			return Null();
		if (forceScalar_) {
			return scalars_->data[0];
		} else {
			FAIL ("vector oper not scalar: " << id_);
		}
	}

	XlpOper::operator std::shared_ptr< Range<Scalar> >() const {
		return scalars_;
	}

	std::string XlpOper::strConv(const OPER *xString) {
		std::wstring ret;
		if (xString->val.str) {
			unsigned char len = xString->val.str[0];
			if (len)
				ret.assign(xString->val.str + 1, len);
		}
		auto rret = ws2s(ret);
		rret = toString(rret);
		return rret;
	}

}



