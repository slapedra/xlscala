#include <limits>
#include <algorithm>
#include <xlscala/xl/xl.hpp>
#include <xlscala/defines.hpp>
#include <xlscala/scala/scalasession.hpp>
#include <xlscala/utilities/null.hpp>

#include <iostream>
#include <fstream>

namespace xlscala {

	XL_EXPORT OPER *xlsSet(OPER* xlScalar, OPER* xlArgs, OPER* xlValues, OPER* xlIndex, OPER* xlDefault, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		Scalar dflt;
		XlpOper xlpScalar(xlScalar, "scalar", true, true);
		XlpOper xlpArgs(xlArgs, "args");
		XlpOper xlpIndex(xlIndex, "index", false, false, true);
		Scalar scalar = xlpScalar.scalar();
		XlpOper xlpDflt(xlDefault, "default", true, true);
		std::shared_ptr< Range<Scalar> > values;
		if (!xlpDflt.missing()) {
		    XlpOper xlpDflt(xlDefault, "default", true, true);
		    dflt = xlpDflt.scalar();
            XlpOper xlpValues(xlValues, "values", dflt, false, false);
		    values = xlpValues;
		} else {
		    XlpOper xlpValues(xlValues, "values", false, false, true);
		    values = xlpValues;
		}
		std::shared_ptr< Range<Scalar> > args = xlpArgs;
		std::shared_ptr< Range<Scalar> > index = xlpIndex;
        auto ret = ScalaSession::instance().set(scalar, args, values, index);
		XL_END(ret)
	}

	XL_EXPORT OPER *xlsExec(OPER* xlCode, OPER* xlArgs, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpCode(xlCode, "code");
		XlpOper xlpArgs(xlArgs, "args");
		std::shared_ptr< Range<Scalar> > args = xlpArgs;
		std::shared_ptr< Range<Scalar> > code = xlpCode;
		auto objs = ScalaSession::instance().exec(code, args, XLSession::instance().transpose());
		std::shared_ptr< Range<Scalar> > datas;
		if (objs->size() == 0) {
			datas.reset(new Range<Scalar>(1, 1));
			datas->data[0] = true /*"success"*/;
		} else {
			datas = objs;
		}
		XL_END(datas)
	}

	XL_EXPORT OPER *xlsExecfile(OPER* xlFilename, OPER* xlArgs, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpFilename(xlFilename, "filename");
		XlpOper xlpArgs(xlArgs, "args");
		std::shared_ptr< Range<Scalar> > args = xlpArgs;
		std::string filename = xlpFilename;
		std::string output;
		std::ifstream file;
		file.open(filename);
		if (!file.is_open()) {
		    std::string path = XLSession::instance().workbookPath();
		    file.open(path + "\\" + filename);
		}
		REQUIRE (file.is_open(), "failed loading file: " << filename)
        std::stringstream code;
        std::string line;
        code << file.rdbuf();
        std::shared_ptr< Range<Scalar> > xcode(new Range<Scalar>(1, 1));
        xcode->data[0] = code.str();
		auto objs = ScalaSession::instance().exec(xcode, args, XLSession::instance().transpose());
		std::shared_ptr< Range<Scalar> > datas;
		if (objs->size() == 0) {
			datas.reset(new Range<Scalar>(1, 1));
			datas->data[0] = true /*"success"*/;
		} else {
			datas = objs;
		}
		XL_END(datas)
	}

	XL_EXPORT OPER *xlsAttr(OPER* xlScalar, OPER* xlAttr, OPER* xlArgs, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpScalar(xlScalar, "scalar", true, true);
		XlpOper xlpArgs(xlArgs, "args");
		XlpOper xlpAttr(xlAttr, "attr", false, false);
		Scalar scalar = xlpScalar.scalar();
		std::string attr = xlpAttr;
		std::shared_ptr< Range<Scalar> > args = xlpArgs;
        auto ret = ScalaSession::instance().attr(scalar, attr, args);
		XL_END(ret)
	}

	XL_EXPORT OPER *xlsImport(OPER* xlScalars, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpScalars(xlScalars, "scalars", true, false);
		std::shared_ptr< Range<Scalar> > scalars = xlpScalars;
		auto key = ScalaSession::instance().use(scalars);
		XL_END(key)
	}

	XL_EXPORT OPER *xlsGet(OPER* xlScalar, OPER* xlArgs, OPER* xlOutput, OPER* xlForce, OPER* xlTranspose, OPER* xlTrigger) {
	    XL_BEGIN_OUTPUT(xlTrigger, xlTranspose, xlOutput)
		bool force = XLSession::instance().convertBool(xlForce, false);
		XlpOper xlpScalar(xlScalar, "scalar", false, false);
		XlpOper xlpArgs(xlArgs, "args");
		Scalar scalar = xlpScalar;
		std::shared_ptr< Range<Scalar> > args = xlpArgs;
		std::shared_ptr< Range<Scalar> > datas = ScalaSession::instance().get(scalar, args, XLSession::instance().transpose(), force);
		XL_END_OUTPUT(datas)
	    /*try {
            XLSession::instance().validateTrigger(xlTrigger);
            bool force = XLSession::instance().convertBool(xlForce, false);
            XlpOper xlpOutput(xlOutput, "output");
            if (xlpOutput.missing()) {
                bool transpose = false;
                if (xlTranspose != NULL) {
                    transpose = XLSession::instance().convertBool(xlTranspose, false);
                }
                XlpOper xlpScalar(xlScalar, "scalar", false, false);
                XlpOper xlpArgs(xlArgs, "args");
                Scalar scalar = xlpScalar.scalar();
                std::shared_ptr< Range<Scalar> > args = xlpArgs;
                std::shared_ptr< Range<Scalar> > datas = ScalaSession::instance().get(scalar, args, force);
                XlpOper ret(datas, "", transpose);
                return ret.get();
			} else {
			    std::shared_ptr< Range<Scalar> > outputs = xlpOutput;
			    if(outputs->size() == 1) {
			        std::string output = xlpOutput;
			        bool transpose = false;
                    if (xlTranspose != NULL) {
                        transpose = XLSession::instance().convertBool(xlTranspose, false);
                    }
			        try {
                        XlpOper xlpScalar(xlScalar, "scalar", false, false);
                        XlpOper xlpArgs(xlArgs, "args");
                        Scalar scalar = xlpScalar.scalar();
                        std::shared_ptr< Range<Scalar> > args = xlpArgs;
                        std::shared_ptr< Range<Scalar> > datas = ScalaSession::instance().get(scalar, args, force);
                        std::string res = XLSession::instance().fillOutput(output, datas, transpose);
                        XlpOper ret(res, "", transpose);
                        return ret.get();
			        } catch (Error& e) {
			            std::shared_ptr< Range<Scalar> > datas(new Range<Scalar>(1,1));
                        datas->data[0] = e;
                        std::string res = XLSession::instance().fillOutput(output, datas, false);
                        XlpOper ret(res, "", transpose);
                        return ret.get();
			        }
                } else {
                    XlpOper xlpScalar(xlScalar, "scalar", false, false);
                    XlpOper xlpTranspose(xlTranspose, "scalar", false, false);
                    XlpOper xlpArgs(xlArgs, "args");
                    std::shared_ptr< Range<Scalar> > transposes = xlpTranspose;
                    std::shared_ptr< Range<Scalar> > scalars = xlpScalar;
                    std::shared_ptr< Range<Scalar> > args = xlpArgs;
                    REQUIRE(outputs->size() == scalars->size(), "scalars and outputs must have same size");
                    if (xlTranspose != NULL) {
                        REQUIRE(outputs->size() == transposes->size(), "scalars and transposes must have same size");
                    }
                    for (Size i=0;i<outputs->size();++i) {
                        try {
                            std::string output = outputs->data[i].get<std::string>();
                            bool transpose = false;
                            if ((xlTranspose != NULL) && (!is_null(transposes->data[i]))) {
                                transpose = transposes->data[i].get<bool>();
                            }
                            Scalar scalar = scalars->data[i];
                            std::shared_ptr< Range<Scalar> > datas = ScalaSession::instance().get(scalar, args, force);
                            XLSession::instance().fillOutput(output, datas, transpose);
                        } catch (Error& e) {
                            std::string output = outputs->data[i].get<std::string>();
                            std::shared_ptr< Range<Scalar> > datas(new Range<Scalar>(1,1));
                            datas->data[0] = e;
                            XLSession::instance().fillOutput(output, datas, false);
                        }
                     }
                    XlpOper ret(true, "", false);
                    return ret.get();
                }
			}
	    } catch (Error &e) {
            XlpOper _ret(e);
            return _ret.get();
	    }*/
	}

	XL_EXPORT OPER *xlsGetAttr(OPER* xlScalar, OPER* xlAttr, OPER* xlArgs,
		OPER* xlOutput,  OPER* xlForce, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_OUTPUT(xlTrigger, xlTranspose, xlOutput)
		bool force = XLSession::instance().convertBool(xlForce, false);
		XlpOper xlpScalar(xlScalar, "scalar", false, false);
		XlpOper xlpAttr(xlAttr, "attr", false, false);
		XlpOper xlpArgs(xlArgs, "args");
		Scalar scalar = xlpScalar;
		std::string attr = "";
		if (xlAttr != NULL) {
		    std::string tmp =  xlpAttr;
		    attr = tmp;
		}
		std::shared_ptr< Range<Scalar> > args = xlpArgs;
		std::shared_ptr< Range<Scalar> > datas = ScalaSession::instance().getAttr(scalar, attr, args, XLSession::instance().transpose(), force);
		XL_END_OUTPUT(datas)
	}

}



