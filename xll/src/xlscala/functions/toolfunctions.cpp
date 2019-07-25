#include <limits>
#include <iostream>
#include <algorithm>
#include <xlscala/xl/xl.hpp>
#include <xlscala/defines.hpp>
#include <xlscala/scala/scalasession.hpp>
#include <boost/timer.hpp>



namespace xlscala {

	XL_EXPORT OPER *xlsArgs(
		OPER* xlArgs01, OPER* xlArgs02, OPER* xlArgs03, OPER* xlArgs04, OPER* xlArgs05,
		OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		std::vector<XlpOper> xlpArgs;
		xlpArgs.push_back(XlpOper(xlArgs01, "args01"));
		xlpArgs.push_back(XlpOper(xlArgs02, "args02"));
		xlpArgs.push_back(XlpOper(xlArgs03, "args03"));
		xlpArgs.push_back(XlpOper(xlArgs04, "args04"));
		xlpArgs.push_back(XlpOper(xlArgs05, "args05"));
		std::vector< std::shared_ptr< Range<Scalar> > > args;
		Size size = 0;
		for (Size i=0;i<xlpArgs.size();i++) {
			std::shared_ptr< Range<Scalar> > tmp = xlpArgs[i];
			args.push_back(tmp);
			size += args[i]->size();
		}
		std::shared_ptr< Range<Scalar> > fullArgs(new Range<Scalar>(1, size));
		Size offset = 0;
		for (Size i=0;i<args.size();i++) {
			for (Size j=0;j<args[i]->size();j++) {
				fullArgs->data[offset] = args[i]->data[j];
				offset++;
			}
		}
		XL_END(fullArgs)
	}

	XL_EXPORT OPER *xlsBigArgs(
		OPER* xlArgs01, OPER* xlArgs02, OPER* xlArgs03, OPER* xlArgs04, OPER* xlArgs05,
		OPER* xlArgs06, OPER* xlArgs07, OPER* xlArgs08, OPER* xlArgs09, OPER* xlArgs10,
		OPER* xlArgs11, OPER* xlArgs12, OPER* xlArgs13, OPER* xlArgs14, OPER* xlArgs15,
		OPER* xlArgs16, OPER* xlArgs17, OPER* xlArgs18, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		std::vector<XlpOper> xlpArgs;
		xlpArgs.push_back(XlpOper(xlArgs01, "args01"));
		xlpArgs.push_back(XlpOper(xlArgs02, "args02"));
		xlpArgs.push_back(XlpOper(xlArgs03, "args03"));
		xlpArgs.push_back(XlpOper(xlArgs04, "args04"));
		xlpArgs.push_back(XlpOper(xlArgs05, "args05"));
		xlpArgs.push_back(XlpOper(xlArgs06, "args06"));
		xlpArgs.push_back(XlpOper(xlArgs07, "args07"));
		xlpArgs.push_back(XlpOper(xlArgs08, "args08"));
		xlpArgs.push_back(XlpOper(xlArgs09, "args09"));
		xlpArgs.push_back(XlpOper(xlArgs10, "args10"));
		xlpArgs.push_back(XlpOper(xlArgs11, "args11"));
		xlpArgs.push_back(XlpOper(xlArgs12, "args12"));
		xlpArgs.push_back(XlpOper(xlArgs13, "args13"));
		xlpArgs.push_back(XlpOper(xlArgs14, "args14"));
		xlpArgs.push_back(XlpOper(xlArgs15, "args15"));
		xlpArgs.push_back(XlpOper(xlArgs16, "args16"));
		xlpArgs.push_back(XlpOper(xlArgs17, "args17"));
		xlpArgs.push_back(XlpOper(xlArgs18, "args18"));
		std::vector< std::shared_ptr< Range<Scalar> > > args;
		Size size = 0;
		for (Size i=0;i<xlpArgs.size();i++) {
			std::shared_ptr< Range<Scalar> > tmp = xlpArgs[i];
			args.push_back(tmp);
			size += args[i]->size();
		}
		std::shared_ptr< Range<Scalar> > fullArgs(new Range<Scalar>(1, size));
		Size offset = 0;
		for (Size i=0;i<args.size();i++) {
			for (Size j=0;j<args[i]->size();j++) {
				fullArgs->data[offset] = args[i]->data[j];
				offset++;
			}
		}
		XL_END(fullArgs)
	}

	XL_EXPORT OPER *xlsTrigger(
		OPER* xlTrig01, OPER* xlTrig02, OPER* xlTrig03, OPER* xlTrig04, OPER* xlTrig05) {
		try {
			std::shared_ptr< Range<Scalar> > datas(new Range<Scalar>(1, 1));
			static xlscala::Integer index = 0;
			try {
				xlscala::XLSession::instance().validateTrigger(xlTrig01);
				xlscala::XLSession::instance().validateTrigger(xlTrig02);
				xlscala::XLSession::instance().validateTrigger(xlTrig03);
				xlscala::XLSession::instance().validateTrigger(xlTrig04);
				xlscala::XLSession::instance().validateTrigger(xlTrig05);
				++index;
				datas->data[0] = index;
			} catch (Error&) {
				datas->data[0] = false;
			}
		XL_END(datas)
	}

	XL_EXPORT OPER *xlsBigTrigger(
		OPER* xlTrig01, OPER* xlTrig02, OPER* xlTrig03, OPER* xlTrig04, OPER* xlTrig05,
		OPER* xlTrig06, OPER* xlTrig07, OPER* xlTrig08, OPER* xlTrig09, OPER* xlTrig10,
		OPER* xlTrig11, OPER* xlTrig12, OPER* xlTrig13, OPER* xlTrig14, OPER* xlTrig15,
		OPER* xlTrig16, OPER* xlTrig17, OPER* xlTrig18, OPER* xlTrig19, OPER* xlTrig20) {
		try {
			std::shared_ptr< Range<Scalar> > datas(new Range<Scalar>(1, 1));
			static xlscala::Integer index = 0;
			try {
				xlscala::XLSession::instance().validateTrigger(xlTrig01);
				xlscala::XLSession::instance().validateTrigger(xlTrig02);
				xlscala::XLSession::instance().validateTrigger(xlTrig03);
				xlscala::XLSession::instance().validateTrigger(xlTrig04);
				xlscala::XLSession::instance().validateTrigger(xlTrig05);
				xlscala::XLSession::instance().validateTrigger(xlTrig06);
				xlscala::XLSession::instance().validateTrigger(xlTrig07);
				xlscala::XLSession::instance().validateTrigger(xlTrig08);
				xlscala::XLSession::instance().validateTrigger(xlTrig09);
				xlscala::XLSession::instance().validateTrigger(xlTrig10);
				xlscala::XLSession::instance().validateTrigger(xlTrig11);
				xlscala::XLSession::instance().validateTrigger(xlTrig12);
				xlscala::XLSession::instance().validateTrigger(xlTrig13);
				xlscala::XLSession::instance().validateTrigger(xlTrig14);
				xlscala::XLSession::instance().validateTrigger(xlTrig15);
				xlscala::XLSession::instance().validateTrigger(xlTrig16);
				xlscala::XLSession::instance().validateTrigger(xlTrig17);
				xlscala::XLSession::instance().validateTrigger(xlTrig18);
				xlscala::XLSession::instance().validateTrigger(xlTrig19);
				xlscala::XLSession::instance().validateTrigger(xlTrig20);
				++index;
				datas->data[0] = index;
			} catch (Error&) {
				datas->data[0] = false;
			}
		XL_END(datas)
	}

	XL_EXPORT OPER *xlsTranspose(OPER* xlScalars, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpTrigger(xlTrigger, "trigger");
		XlpOper xlpScalars(xlScalars, "scalars", true);
		std::shared_ptr< Range<Scalar> > scalars = xlpScalars;
		XL_END(scalars)
	}

	XL_EXPORT OPER *xlsAdjustShape(OPER* xlScalars, OPER* xlRows, OPER* xlColumns, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpColumns(xlColumns, "columns");
		XlpOper xlpRows(xlRows, "rows");
		XlpOper xlpScalars(xlScalars, "scalars");
		xlscala::Integer rows = xlpRows;
		xlscala::Integer columns = xlpColumns;
		std::shared_ptr< Range<Scalar> > scalars = xlpScalars;
		scalars->rows = rows;
		scalars->columns = columns;
		REQUIRE (Size(columns*rows) == scalars->size(), "bad size for new shape: "
			<< rows*columns << " != " << scalars->size());
		XL_END(scalars)
	}

	XL_EXPORT OPER *xlsVolatile(OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		static xlscala::Integer index = 0;
		index++;
		XL_END(index)
	}

	XL_EXPORT OPER *xlsReduce(OPER* xlArgs, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpArgs(xlArgs, "args", false, false, true);
		std::shared_ptr< Range<Scalar> > res = xlpArgs;
		XL_END(res)
	}

	XL_EXPORT OPER *xlsPretty(OPER* xlScalars, OPER* xlTranspose, OPER * xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpScalars(xlScalars, "scalars");
		std::shared_ptr< Range<Scalar> > scalars = xlpScalars;
		XL_END_PRETTY(scalars)
	}

	XL_EXPORT OPER *xlsFalseTrigger(OPER* xlScalar, OPER* xlFalse, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpScalar(xlScalar, "scalar");
		XlpOper xlpFalse(xlFalse, "false");
		Scalar falsevalue;
		if (xlpFalse.missing()) {
			falsevalue = false;
		} else {
			falsevalue = xlpFalse.scalar();
		}
		Scalar scalar = xlpScalar.scalar();
		XL_END(!(scalar == falsevalue))
	}

	XL_EXPORT OPER *xlsName(OPER* xlRef, OPER* xlName, OPER * xlTrigger) {
		XL_BEGIN(xlTrigger)
		static XLOPER12 xlBool;
		xlBool.xltype = xltypeBool;
		xlBool.val.xbool = false;
		Excel12(xlfVolatile, 0, 1, &xlBool);
		static XLOPER12 xlInt;
		xlInt.xltype = xltypeInt;
		xlInt.val.w = xltypeRef;
		std::shared_ptr<Xloper> xllRef(new Xloper);
		int result = Excel12(xlCoerce, &(*xllRef), 2, xlRef, &xlInt);
		Scalar ret = Null();
		if (result != xlretSuccess) {
			ret = "ERRORO";
			//std::cerr << "filling warning message error" << std::endl;
		} else {
			ret = true /*"success"*/;
			std::shared_ptr<XLRef> ref(new XLRef(xllRef));
			XlpOper xlpName(xlName, "name");
			std::string name = xlpName;
			XLSession::instance().defineName(ref, name);
		}
		XL_END(ret)
    }

    XL_EXPORT OPER *xlsMessage(OPER* xlMsg, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpMsg(xlMsg, "msg");
		std::string msg = xlpMsg;
		XLSession::instance().message(msg);
		XL_END(true/*"success"*/)
	}

	XL_EXPORT OPER *xlsWarning(OPER* xlMsg, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpMsg(xlMsg, "msg");
		std::string msg = xlpMsg;
		XLSession::instance().warning(msg);
		XL_END(true/*"success"*/)
	}

	XL_EXPORT OPER *xlsError(OPER* xlMsg, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpMsg(xlMsg, "msg");
		std::string msg = xlpMsg;
		XLSession::instance().error(msg);
		XL_END(true/*"success"*/)
	}

}



