#include <xlscala/xl/xlsession.hpp>
#include <xlscala/xl/xlpoper.hpp>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <boost/xpressive/xpressive_static.hpp>
#include <xlscala/version.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <xlscala/xl/xlcommandevents/all.hpp>
#include <xlscala/xl/xlcommandevent.hpp>
#include <xlscala/scala/scalasession.hpp>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <boost/timer.hpp>

using namespace boost::xpressive;

namespace xlscala {

/*****************************************************************************************************************
	XLCurrentPosition
*****************************************************************************************************************/
	XLCurrentPosition::XLCurrentPosition(long id, Size row, Size col, const std::string& addr,
		const std::string& workbook, bool transpose)
	: transpose_(transpose), curIdSheet_(id), curRow_(row), curCol_(col), addr_(addr), workbook_(workbook) {
	}

	void XLCurrentPosition::addRow() const {
		if (transpose_) {
			curCol_++;
		} else {
			curRow_++;
		}
	}

	void XLCurrentPosition::addCol() const{
		if (!transpose_) {
			curCol_++;
		} else {
			curRow_++;
		}
	}

	/*! XLCaller
	 */
	XLCaller::XLCaller() {
		static XLOPER12 xlInt;
		xlInt.xltype = xltypeInt;
		xlInt.val.w = xltypeRef;
		Xloper xlCaller;
		int result = Excel12(xlfCaller, &xlCaller, 0);
		if (result != xlretSuccess) {
			FAIL("xlfCaller error");
		}
		Xloper xlRef;
		result = Excel12(xlCoerce, &xlRef, 2, &xlCaller, &xlInt);
		if (result != xlretSuccess) {
			FAIL("ref conversion failed");
		}
		REQUIRE (xlRef->val.mref.lpmref->count == 1, "multiple reference not supported");
		idSheet_ = xlRef->val.mref.idSheet;
		row_ = xlRef->val.mref.lpmref->reftbl[0].rwFirst;
		rows_ = xlRef->val.mref.lpmref->reftbl[0].rwLast - row_ + 1;
		col_ = xlRef->val.mref.lpmref->reftbl[0].colFirst;
		cols_ = xlRef->val.mref.lpmref->reftbl[0].colLast - col_ + 1;
		Xloper xlAddr;
		xlInt.val.w = 32;
		result = Excel12(xlfGetCell, &xlAddr, 2, &xlInt, &xlRef);
		if ( (result != xlretSuccess) || (xlAddr->xltype == xltypeErr) ) {
			FAIL("error getting address of caller");
		}
		XlpOper xlpName(&xlAddr, "addr");
		addr_ = std::string(xlpName);
		std::vector<std::string> ret;
		boost::split(ret, addr_, boost::is_any_of("]"));
		if (ret.size()) {
			workbook_ = ret[0];
			trim_left_if(workbook_, boost::is_any_of("'"));
			trim_left_if(workbook_, boost::is_any_of("["));
		} else {
			workbook_ = addr_;
		}
	}

	/***********************************************
		XLFunction
	***********************************************/
	XLFunction::XLFunction(
		const std::wstring& moduleName,
		const std::wstring& cName, const std::wstring& cArgs,
		const std::wstring& xlName, const std::wstring& xlArgs,
		const std::wstring& desc,
		const std::wstring& descArgs)
	: cName_(cName), cArgs_(cArgs), xlName_(xlName),
      xlArgs_(xlArgs), desc_(desc),
	  moduleName_(moduleName) {
		typedef boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring> tokenizer;
		boost::char_separator<wchar_t> sep(L"\n");
		std::wstring tmp = descArgs;
		tmp += L" ";
		tokenizer tokens(tmp, sep);
		for (tokenizer::iterator tok_iter = tokens.begin();
			tok_iter != tokens.end(); ++tok_iter) {
			descArgs_.push_back(*tok_iter);
		}
	}

	void XLFunction::registerFunction() {
		XLOPER12 xResult;
		XLOPER12 xDll;
		Excel12(xlGetName, &xDll, 0);
		std::vector<LPXLOPER12> args;
		args.push_back((LPXLOPER12)&xDll);
		// function code name
		args.push_back(TempStr12(cName_.c_str()));
		// parameter codes
		args.push_back(TempStr12(cArgs_.c_str()));
		// display name
		args.push_back(TempStr12(xlName_.c_str()));
		// comma-delimited list of parameter names
		args.push_back(TempStr12(xlArgs_.c_str()));
		// function type (0 = hidden, 1 = worksheet, 2 = type3)
		args.push_back(TempNum12(1));
		// module name
		args.push_back(TempStr12(moduleName_.c_str()));
		// shortcut text for macro function
		args.push_back(TempStr12(L""));
		// path to help file
		args.push_back(TempStr12(L""));
		// function description
		args.push_back(TempStr12(desc_.c_str()));
		// arguments description*/
		for (Size i=0;i<descArgs_.size();i++) {
			args.push_back(TempStr12(descArgs_[i].c_str()));
		}
		Integer res = Excel12v(xlfRegister, &xResult,
			static_cast<int>(args.size()), args.data());
		if ( (res != xlretSuccess) || (xResult.xltype == xltypeErr) ) {
			Excel12(xlFree, 0, 1, &xResult);
			Excel12(xlFree, 0, 1, &xDll);
			FAIL ("unable to register function : " << ws2s(xlName_));
		}
		regId_ = xResult.val.num;
		Excel12(xlFree, 0, 1, &xResult);
		Excel12(xlFree, 0, 1, &xDll);
	}

	void XLFunction::unregisterFunction() {
		int result;
		if (regId_) {
			result = Excel12(xlfUnregister, 0, 1, TempNum12(regId_));
			REQUIRE (result == xlretSuccess, "unable to unregister function : " << ws2s(xlName_));
		}
	}

	/*******************************************************
		XLCommand
	*******************************************************/
	XLCommand::XLCommand(
		const std::wstring& moduleName,
		const std::wstring& cName, const std::wstring& cArgs,
		const std::wstring& xlName, const std::wstring& xlArgs,
		const std::wstring& desc,
		const std::wstring& descArgs)
	: cName_(cName), cArgs_(cArgs), xlName_(xlName),
    xlArgs_(xlArgs), desc_(desc),
	  moduleName_(moduleName) {
		typedef boost::tokenizer<boost::char_separator<wchar_t>, std::wstring::const_iterator, std::wstring> tokenizer;
		boost::char_separator<wchar_t> sep(L"\n");
		std::wstring tmp = descArgs;
		tmp += L" ";
		tokenizer tokens(tmp, sep);
		for (tokenizer::iterator tok_iter = tokens.begin();
			tok_iter != tokens.end(); ++tok_iter) {
			descArgs_.push_back(*tok_iter);
		}
	}


	void XLCommand::registerCommand() {
		XLOPER12 xResult;
		XLOPER12 xDll;
		Excel12(xlGetName, &xDll, 0);
		std::vector<LPXLOPER12> args;
		args.push_back((LPXLOPER12)&xDll);
		// function code name
		args.push_back(TempStr12(cName_.c_str()));
		// parameter codes
		args.push_back(TempStr12(cArgs_.c_str()));
		// display name
		args.push_back(TempStr12(xlName_.c_str()));
		// comma-delimited list of parameter names
		args.push_back(TempStr12(xlArgs_.c_str()));
		// function type (0 = hidden, 1 = worksheet, 2 = type3)
		args.push_back(TempNum12(2));
		// module name
		args.push_back(TempStr12(moduleName_.c_str()));
		// shortcut text for macro function
		args.push_back(TempStr12(L""));
		// path to help file
		args.push_back(TempStr12(L""));
		// function description
		args.push_back(TempStr12(desc_.c_str()));
		// arguments description
		for (Size i=0;i<descArgs_.size();i++) {
			args.push_back(TempStr12(descArgs_[i].c_str()));
		}
		Integer res = Excel12v(xlfRegister, &xResult,
			static_cast<int>(args.size()), args.data());
		if ( (res != xlretSuccess) || (xResult.xltype == xltypeErr) ) {
			Excel12(xlFree, 0, 1, &xResult);
			Excel12(xlFree, 0, 1, &xDll);
			FAIL ("unable to register command : " << ws2s(xlName_));
		}
		regId_ = xResult.val.num;
		Excel12(xlFree, 0, 1, &xResult);
		Excel12(xlFree, 0, 1, &xDll);
	}

	void XLCommand::unregisterCommand() {
		int result;
		if (regId_) {
			result = Excel12(xlfUnregister, 0, 1, TempNum12(regId_));
			REQUIRE (result == xlretSuccess, "unable to unregister command: " << ws2s(xlName_));
		}
	}



namespace details {

		#define SECS_PER_DAY (60.0 * 60.0 * 24.0)

        XL_EXPORT bool xlOnHideProtect(void) {
			try {
				XLSession::instance().freeze(true);
				XLSession::instance().onHideProtect(true);
				XLSession::instance().freeze(false);
				return true;
			} catch(Error& e) {
				XLSession::instance().freeze(false);
				std::cerr << e.what() << std::endl;
				return false;
			}
		}

		XL_EXPORT bool xlOnUnhideUnprotect(void) {
			try {
				XLSession::instance().freeze(true);
				XLSession::instance().onHideProtect(false);
				XLSession::instance().freeze(false);
				return true;
			} catch(Error& e) {
				XLSession::instance().freeze(false);
				std::cerr << e.what() << std::endl;
				return false;
			}
		}

		static LPWSTR menu[][2] = {
			{(wchar_t*)L" &XLScala",				                                                    (wchar_t*)L" "},
            {(wchar_t*)L" &hide/protect",				                                                (wchar_t*)L" xlOnHideProtect"},
            {(wchar_t*)L" &unhide/unprotect",			                                                (wchar_t*)L" xlOnUnhideUnprotect"},
			{(wchar_t*)L" -",							                                                (wchar_t*)L" "},
			{(wchar_t*)L" version: "XLSCALA_WSTR_PRODUCTVERSION,                                         (wchar_t*)L" "},
			{(wchar_t*)L" author: Sebastien Lapedra",		                                            (wchar_t*)L" "},
			{(wchar_t*)L" host:",	                    	                                            (wchar_t*)L" "},
			{(wchar_t*)L" port:",		                                                                (wchar_t*)L" "}

		};

		static XLOPER12 xMenu;
		static XLOPER12 xMenuList[8*2];


		XL_EXPORT bool xlOnRecalc(void) {
			try {
                XLSession::instance().onRecalc();
			    return true;
			} catch (Error& e) {
				std::cerr << e.what() << std::endl;
				return false;
			}
		}

	}
	/*******************************************************
		XLSession
	*******************************************************/
	XLSession::~XLSession() {
	}

	void XLSession::registerSession() {
	    registerXLCommand(
			L"XLL",
			L"xlOnRecalc",
			XL_BOOL+XL_UNCALCED,
			L"xlOnRecalc",
			L"",
			L"",
			L"");

        registerXLCommand(
			L"XLL",
			L"xlOnHideProtect",
			XL_BOOL+XL_UNCALCED,
			L"xlOnHideProtect",
			L"",
			L"",
			L"");
		registerXLCommand(
			L"XLL",
			L"xlOnUnhideUnprotect",
			XL_BOOL+XL_UNCALCED,
			L"xlOnUnhideUnprotect",
			L"",
			L"",
			L"");
		XLOPER12 xDll;
		int result = xlretSuccess;
		result  = Excel12(xlGetName, &xDll, 0);
		if (result != xlretSuccess) {
			Excel12(xlFree, 0, 1, &xDll);
			FAIL ("xlGetName error");
		} else {
			XlpOper xlaId(&xDll, "id");
			id_ = std::string(xlaId);
			for (Size i=0;i<functions_.size();i++) {
				functions_[i]->registerFunction();
			}
			for (Size i=0;i<commands_.size();i++) {
				commands_[i]->registerCommand();
			}
			Excel12(xlFree, 0, 1, &xDll);
			int res = Excel12(xlcOnRecalc, 0, 2, 0, TempStr12(L"xlOnRecalc"));
			if (res != xlretSuccess) {
				FAIL("xlcOnRecalc");
			}
        }

		details::xMenu.xltype            = xltypeMulti;
        details::xMenu.val.array.lparray = &details::xMenuList[0];
        details::xMenu.val.array.rows    = 8;
        details::xMenu.val.array.columns = 2;
        for(int i=0;i<details::xMenu.val.array.rows;++i) {
            for(int j=0;j<details::xMenu.val.array.columns;++j) {
                details::xMenuList[j+i*2].xltype  = xltypeStr;
                details::xMenuList[j+i*2].val.str = details::menu[i][j];
            }
        }
        auto host =  std::wstring(L" host: ") + s2ws(ScalaSession::instance().host());
        wchar_t *hhost = new wchar_t[host.size()+1];
        for (Size i=0;i<host.size();++i) hhost[i] = host[i];
        hhost[host.size()] = '\0';
        details::xMenuList[2*6].val.str = hhost;

        auto port = std::wstring(L" port: ") + boost::lexical_cast<std::wstring>(ScalaSession::instance().port());
        wchar_t *pport = new wchar_t[port.size()+1];
        for (Size i=0;i<port.size();++i) pport[i] = port[i];
        pport[port.size()] = '\0';
        details::xMenuList[2*7].val.str = pport;

        Excel12(xlfAddMenu,0, 2, TempNum12(1), (LPXLOPER12)&details::xMenu);
        Excel12(xlfEnableCommand,0, 4, TempNum12(1), &details::xMenuList[0], &details::xMenuList[8], TempBool12(false));
        Excel12(xlfEnableCommand,0, 4, TempNum12(1), &details::xMenuList[0], &details::xMenuList[10], TempBool12(false));
        Excel12(xlfEnableCommand,0, 4, TempNum12(1), &details::xMenuList[0], &details::xMenuList[12], TempBool12(false));
        Excel12(xlfEnableCommand,0, 4, TempNum12(1), &details::xMenuList[0], &details::xMenuList[14], TempBool12(false));
	}

	void XLSession::unregisterSession() {
		for (Size i=0;i<functions_.size();i++) {
			functions_[i]->unregisterFunction();
		}
		for (Size i=0;i<commands_.size();i++) {
			commands_[i]->unregisterCommand();
		}
	}

	void XLSession::push_back(const std::shared_ptr<XLFunction>& func) {
		functions_.push_back(func);
	}

	void XLSession::push_back(const std::shared_ptr<XLCommand>& command) {
		commands_.push_back(command);
	}

	std::shared_ptr<XLFunction> function(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs) {
		std::shared_ptr<XLFunction> ret(new XLFunction(moduleName, cName, cArgs,
			xlName, xlArgs, desc, descArgs));
		return ret;
	}

	std::shared_ptr<XLCommand> command(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs) {
		std::shared_ptr<XLCommand> ret(new XLCommand(moduleName, cName, cArgs,
			xlName, xlArgs, desc, descArgs));
		return ret;
	}

	void registerXLFunction(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs) {
				xlscala::XLSession::instance().push_back(
					xlscala::function(moduleName, cName, cArgs,
						xlName, xlArgs, desc, descArgs));
	}

	void registerXLCommand(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs) {
				xlscala::XLSession::instance().push_back(
					command(moduleName, cName, cArgs,
						xlName, xlArgs, desc, descArgs));
	}

	void XLSession::validateTrigger(OPER* xlTrigger) {
        XlpOper xlpTrigger(xlTrigger, "trigger");
        bool flag = true;
        try {
            flag = xlpTrigger;
        } catch (Error&) {
        }
        if (flag == false) {
            FAIL ("invalidated function");
        }
	}

	bool XLSession::convertBool(OPER* xlValue, bool defaultValue) {
		XlpOper xlpValue(xlValue, "value", true, false);
		bool value = defaultValue;
		if (!xlpValue.missing()) {
			value = xlpValue;
		}
		return value;
	}

	std::shared_ptr<XLRef> XLSession::checkOutput(const std::string& output) const {
		static XLOPER12 xlBool;
		static XLOPER12 xlInt;
		XlpOper xlpId(output, "id");
		xlpId.setToFree(true);
		std::shared_ptr<Xloper> xlOutput(new Xloper());
		xlBool.xltype = xltypeBool;
		xlBool.val.xbool = true;
		xlInt.xltype = xltypeInt;
		xlInt.val.w = xltypeRef;
		std::shared_ptr<Xloper> xlRef(new Xloper());
		Excel12(xlfTextref, &(*(xlRef.get())), 2, xlpId.get(), &xlBool);
		if ((*(xlRef.get()))->xltype == xltypeRef) {
			std::shared_ptr<XLRef> ref(new XLRef(xlRef));
			return ref;
		} else {
			int result = Excel12(xlCoerce, &(*xlOutput), 2, &(*(xlRef.get())), &xlInt);
			if ( (result != xlretSuccess) || ( (&(*xlOutput))->xltype == xltypeErr) ) {
				FAIL("error getting address of name matrix cell: " << output);
			}
			std::shared_ptr<XLRef> ref(new XLRef(xlOutput));
			return ref;
		}
	}

	std::string XLSession::fillOutput(const std::shared_ptr< Range<Scalar> >& data) const {
	    if ( (output_ == "inplace") && (xlref_.get() != NULL)) {
			std::shared_ptr<XLCommandEvent> com(new XLInplaceOutputEvent(data, transpose_, xlref_, this));
			recalcEvents_.push_back(com);
			return "success";
		} else if ( (!output_.empty()) && (xlref_.get() != NULL)) {
			std::shared_ptr<XLCommandEvent> com(new XLOutputEvent(data, transpose_, xlref_, this));
			recalcEvents_.push_back(com);
			if (!transpose_) {
				if ( (xlref_->cols() < data->columns) || (xlref_->rows() < data->rows)) {
					return "more";
				}
			} else {
				if ( (xlref_->cols() < data->rows) || (xlref_->rows() < data->columns)) {
					return "more";
				}
			}
			return "success";
		} else {
			return "";
		}
	}

	std::string XLSession::fillOutput(const std::shared_ptr<XLRef>& xlref,
		const std::shared_ptr< Range<Scalar> >& data, bool transpose, bool inplace = false) const {
		if ((xlref.get() != NULL)) {
		    if (!inplace) {
                std::shared_ptr<XLCommandEvent> com(new XLOutputEvent(data, transpose, xlref, this));
                recalcEvents_.push_back(com);
                if (!transpose) {
                    if ( (xlref->cols() < data->columns) || (xlref->rows() < data->rows)) {
                        return "more";
                    }
                } else {
                    if ( (xlref->cols() < data->rows) || (xlref->rows() < data->columns)) {
                        return "more";
                    }
                }
                return "success";
		    } else {
		        std::shared_ptr<XLCommandEvent> com(new XLInplaceOutputEvent(data, transpose, xlref, this));
                recalcEvents_.push_back(com);
                return "success";
		    }
		} else {
			return "";
		}
	}

	std::string XLSession::fillOutput(const std::string& output, const std::shared_ptr< Range<Scalar> >& data, bool transpose) const {
	    XLCaller caller;
	    std::shared_ptr<XLRef> xlref;
	    if (output == "inplace") {
            xlref.reset(new XLRef(caller.row(), caller.col(), 1, 1, caller.idSheet()));
            std::shared_ptr<XLCommandEvent> com(new XLInplaceOutputEvent(data, transpose, xlref, this));
            recalcEvents_.push_back(com);
            return "success";
        } else {
            std::string id = "'"+caller.addr()+"'!"+output;
            xlref = XLSession::instance().checkOutput(id);
            std::shared_ptr<XLCommandEvent> com(new XLOutputEvent(data, transpose, xlref, this));
            recalcEvents_.push_back(com);
            if (!transpose) {
                if ( (xlref->cols() < data->columns) || (xlref->rows() < data->rows)) {
                    return "more";
                }
            } else {
                if ( (xlref->cols() < data->rows) || (xlref->rows() < data->columns)) {
                    return "more";
                }
            }
            return "success";
        }
	}

	void XLSession::freeze(bool newCalcSettingFlag, bool display) const {
		static bool oldCalcSettingFlag = false;
		static XLOPER12 xlOldCalcSetting;
		if (newCalcSettingFlag) {
			if (display) {
				static XLOPER12 xlBool;
				xlBool.xltype = xltypeBool;
				xlBool.val.xbool = false;
				int result = Excel12(xlcEcho, 0, 1, &xlBool);
				if (result != xlretSuccess) {
					FAIL("error in freezing screen update");
				}
			}
			if (oldCalcSettingFlag != newCalcSettingFlag) {
				static XLOPER12 xlArg;
				xlArg.xltype = xltypeInt;
				xlArg.val.w = 14;
				int result = Excel12(xlfGetDocument, &xlOldCalcSetting, 1, &xlArg);
				if (result != xlretSuccess) {
					FAIL("error in old calculation setting");
				}
				oldCalcSettingFlag = newCalcSettingFlag;
			}
			static XLOPER12 xlCalcSetting;
			xlCalcSetting.xltype = xltypeNum;
			xlCalcSetting.val.num = 3;
			int result = Excel12(xlcCalculation, 0, 1, &xlCalcSetting);
			if (result != xlretSuccess) {
				FAIL("error in changing calculation setting");
			}
		} else {
			if (oldCalcSettingFlag) {
				int result = Excel12(xlcCalculation, 0, 1, &xlOldCalcSetting);
				if (result != xlretSuccess) {
					FAIL("error in setting old calculation setting");
				}
				oldCalcSettingFlag = false;
			}
		}
	}

	void XLSession::enterFunction(OPER* xlTrigger, OPER* xlTranspose, OPER* xlOutput) const {
		flagCurrentPosition_ = false;
		XLSession::instance().validateTrigger(xlTrigger);
		if (xlTranspose != NULL) {
			transpose_ = XLSession::instance().convertBool(xlTranspose, false);
		} else {
			transpose_ = false;
		}
		if (xlOutput != NULL) {
			XlpOper xlpOutput(xlOutput, "output");
			if (!xlpOutput.missing()) {
				output_ = std::string(xlpOutput);
				XLSession::instance().currentPosition();
			} else {
				output_ = "";
			}
		} else {
			output_ = "";
		}
	}

	void XLSession::exitFunction() const {
		flagCurrentPosition_ = false;
		transpose_ = false;
		output_ = "";
	}

	std::shared_ptr<XLCurrentPosition> XLSession::currentPosition() const {
		if (!flagCurrentPosition_) {
			XLCaller caller;
			if (output_.empty()) {
				currentPosition_.reset(
					new XLCurrentPosition(caller.idSheet(), caller.row(), caller.col(), caller.addr(),
						caller.workbook(), transpose_));
			} else if (output_ == "inplace") {
                xlref_.reset(new XLRef(caller.row(), caller.col(), 1, 1, caller.idSheet()));
                currentPosition_.reset(
					new XLCurrentPosition(caller.idSheet(), caller.row(), caller.col(), caller.addr(),
						caller.workbook(), transpose_));
            } else {
				std::string id = "'"+caller.addr()+"'!"+output_;
				xlref_ = XLSession::instance().checkOutput(id);
				currentPosition_.reset(
					new XLCurrentPosition(xlref_->idSheet(), xlref_->row(), xlref_->col(), caller.addr(),
					caller.workbook(), transpose_));
			}
			flagCurrentPosition_ = true;
		}
		return currentPosition_;
	}

	std::shared_ptr<XLRef> XLSession::currentRef(const XLCaller& caller, const std::string& output) const {
		REQUIRE(!output.empty(), "empty ouput");
		std::shared_ptr<XLRef> ref;
		std::string id = "'"+caller.addr()+"'!"+output;
		return XLSession::instance().checkOutput(id);
	}

	std::string XLSession::workbookPath() const {
		XLCaller caller;
		static XLOPER12 xlInt;
		xlInt.xltype = xltypeInt;
		xlInt.val.w = 2;
		XlpOper xlpName(caller.addr());
		Xloper xlPath;
		int result = Excel12(xlfGetDocument, &xlPath, 2, &xlInt, xlpName.get());
		if ( (result != xlretSuccess) || (xlPath->xltype == xltypeErr) ) {
			FAIL("error getting path of the workbook");
		}
		XlpOper xlpPath(&xlPath, "path");
		return std::string(xlpPath);
	}

	std::string XLSession::workbookName() const {
		XLCaller caller;
		static XLOPER12 xlInt;
		xlInt.xltype = xltypeInt;
		xlInt.val.w = 88;
		XlpOper xlpName(caller.addr());
		Xloper xlPath;
		int result = Excel12(xlfGetDocument, &xlPath, 2, &xlInt, xlpName.get());
		if ( (result != xlretSuccess) || (xlPath->xltype == xltypeErr) ) {
			FAIL("error getting name of the workbook");
		}
		XlpOper xlpPath(&xlPath, "path");
		return std::string(xlpPath);
	}

	std::string XLSession::worksheetName(bool little) const {
		XLCaller caller;
		if (little) {
			std::vector<std::string> ret;
			boost::split(ret, caller.addr(), boost::is_any_of("]"));
			return ret[ret.size()-1];
		} else {
			return caller.addr();
		}
	}


	void XLSession::calculateNow() const {
		Excel12(xlcCalculateNow, 0, 0);
	}

	void XLSession::onRecalc() const {
		while (recalcEvents_.size() != 0) {
			freeze(true);
			std::list< std::shared_ptr<XLRef> > xlRefs;
			std::list< std::shared_ptr<XLCommandEvent> >::iterator iter = recalcEvents_.begin();
			std::shared_ptr<XLRef> xlRef;
			while (iter != recalcEvents_.end()) {
				if ( (*iter)->execute(xlRef) ) {
					break;
				}
				++iter;
				if ( (xlRef.get()) != NULL ) {
					xlRefs.push_back(xlRef);
				}
			}
			recalcEvents_.erase(recalcEvents_.begin(), recalcEvents_.end());
			freeze(false);
		}
		FreeAllTempMemory();
	}

	void XLSession::onHideProtect(bool flag) const {
		static XLOPER12 xlInt;
		static XLOPER12 xlBool;
		xlInt.xltype = xltypeInt;
		xlBool.xltype = xltypeBool;
		// active sheet
		xlInt.val.w = 38;
		Xloper xlSheet;
		Excel12(xlfGetWorkbook, &xlSheet, 1, &xlInt);
		XlpOper xlpSheet(&xlSheet, "sheets");
		std::string activesheet = xlpSheet;
		// sheets
		xlInt.val.w = 1;
		Xloper xlSheets;
		Excel12(xlfGetWorkbook, &xlSheets, 1, &xlInt);
		XlpOper xlpSheets(&xlSheets, "sheets");
		std::shared_ptr< Range<Scalar> > sheets = xlpSheets;
		// src
		xlInt.val.w = 88;
		Xloper xlPath;
		Excel12(xlfGetDocument, &xlPath, 1, &xlInt);
		XlpOper xlpPath(&xlPath, "path");
		std::string src = xlpPath;
		// unprotect workbook
		xlBool.val.xbool = false;
		Excel12(xlcWorkbookProtect, 0, 1, &xlBool);
		// unprotect and hide
		for (Size i=0;i<sheets->size();++i) {
			std::string sheet = sheets->data[i];
			std::vector<std::string> ret;
			boost::split(ret, sheet, boost::is_any_of("]"));
			sheet = ret[1];
			Excel12(xlcWorkbookSelect, 0, 1, TempStrStl(sheet));
			// unprotect
			xlBool.val.xbool = false;
			Excel12(xlcProtectDocument, 0, 1, &xlBool);
			// unhide
			Excel12(xlcWorkbookUnhide, 0, 0);
		}
		// cleaning names;
		Xloper xlNames;
		int result;
		result = Excel12(xlfNames, &xlNames, 0);
		if (result != xlretSuccess) {
			std::cerr << "error in getting names list" << std::endl;;
		} else {
			XlpOper xlpNames(&xlNames, "names");
			std::shared_ptr< Range<Scalar> > names = xlpNames;
			for (Size j=0;j<names->size();++j) {
				std::string name = names->data[j];
				try {
					std::shared_ptr<XLRef> xlRef = checkOutput("'"+src+"'!"+name);
				} catch (...) {
					Excel12(xlcDeleteName, 0, 1, TempStrStl(name));
				}
			}
		}
		// hide / unhide columns rows
		for (Size i=0;i<sheets->size();++i) {
			std::string sheet = sheets->data[i];
			std::vector<std::string> ret;
			boost::split(ret, sheet, boost::is_any_of("]"));
			sheet = ret[1];
			Excel12(xlcWorkbookSelect, 0, 1, TempStrStl(sheet));
			std::shared_ptr<XLRef> xlRefA1 = checkOutput("'["+src+"]"+sheet+"'!A1");
			Xloper xlNames;
			int result;
			result = Excel12(xlfNames, &xlNames, 0);
			if (result != xlretSuccess) {
				std::cerr << "error in getting names list" << std::endl;;
			} else {
				XlpOper xlpNames(&xlNames, "names");
				std::shared_ptr< Range<Scalar> > names = xlpNames;
				for (Size j=0;j<names->size();++j) {
					std::string name = names->data[j];
					static sregex regstr = as_xpr("xls_code_") >> (s1 = +boost::xpressive::_);
					smatch what;
					if (flag) {
						xlInt.val.w = 1;
					} else {
						xlInt.val.w = 2;
					}
					if (regex_match(name, what, regstr)) {
						std::string key = what[1];
						std::shared_ptr<XLRef> xlRef = checkOutput("'"+src+"'!"+name);
						if (xlRef->idSheet() == xlRefA1->idSheet()) {
							Excel12(xlcColumnWidth, 0, 4, NULL, &(*(xlRef->oper())), NULL, &xlInt);
						}
					}
				}
			}
		}
		// unprotect or protect
		for (Size i=0;i<sheets->size();++i) {
			std::string sheet = sheets->data[i];
			std::vector<std::string> ret;
			boost::split(ret, sheet, boost::is_any_of("]"));
			sheet = ret[1];
			xlBool.val.xbool = flag;
			Excel12(xlcWorkbookSelect, 0, 1, TempStrStl(sheet));
			// protect
			Excel12(xlcProtectDocument, 0, 1, &xlBool);
			// hide
			static sregex regstr = as_xpr("xls_") >> (s1 = +boost::xpressive::_);
			smatch what;
			if (regex_match(sheet, what, regstr)) {
				std::string key = what[1];
				if (flag) {
					Excel12(xlcWorkbookHide, 0, 0);
				} else {
					Excel12(xlcWorkbookUnhide, 0, 0);
				}
			}
		}
		xlBool.val.xbool = flag;
		Excel12(xlcWorkbookSelect, 0, 1, TempStrStl(activesheet));
		Excel12(xlcWorkbookProtect, 0, 1, &xlBool);
		FreeAllTempMemory();
	}


	void XLSession::defineName(const std::string& scalar, const std::string& name) const {
		currentPosition();
		std::string id = "'"+currentPosition_->addr()+"'!"+scalar;
		static XLOPER12 xlBool;
		static XLOPER12 xlInt;
		XlpOper xlpId(id, "id");
		xlpId.setToFree(true);
		Xloper xlTmpRef;
		std::shared_ptr<Xloper> xlRef(new Xloper());
		xlBool.xltype = xltypeBool;
		xlBool.val.xbool = true;
		xlInt.xltype = xltypeInt;
		xlInt.val.w = xltypeRef;
		Excel12(xlfTextref, &xlTmpRef, 2, xlpId.get(), &xlBool);
		int result = Excel12(xlCoerce, &(*xlRef), 2, &xlTmpRef, &xlInt);
		if ( (result != xlretSuccess) || ( (&(*xlRef))->xltype == xltypeErr) ) {
			FAIL("error getting address of name matrix cell: " << scalar);
		}
		XlpOper xlpMulti(&(*xlRef), "multi", false, false, true);
		std::shared_ptr< Range<Scalar> > data = xlpMulti;
		(&(*xlRef))->val.mref.lpmref->reftbl[0].rwLast =
			(&(*xlRef))->val.mref.lpmref->reftbl[0].rwFirst + data->rows-1;
		(&(*xlRef))->val.mref.lpmref->reftbl[0].colLast =
			(&(*xlRef))->val.mref.lpmref->reftbl[0].colFirst + data->columns-1;
		std::shared_ptr<XLRef> ref(new XLRef(xlRef));
		std::shared_ptr<XLCommandEvent> com(new XLNameEvent(name, ref, this));
		recalcEvents_.push_back(com);
	}

	void XLSession::defineName(const std::shared_ptr<XLRef>& xlRef, const std::string& name) const {
		XlpOper xlpMulti(&(*(xlRef->oper())), "multi", false, false, true);
		std::shared_ptr< Range<Scalar> > data = xlpMulti;
		std::shared_ptr<XLRef> ref(new XLRef(
			xlRef->row(), xlRef->col(),
			data->rows, data->columns,
			xlRef->idSheet()));
		std::shared_ptr<XLCommandEvent> com(new XLNameEvent(name, ref, this));
		recalcEvents_.push_back(com);
	}

	void XLSession::message(const std::string& str) const {
		std::shared_ptr<XLCommandEvent> com(new XLMessageEvent(str, this));
		recalcEvents_.push_back(com);
	}

	void XLSession::error(const std::string& str) const {
		std::shared_ptr<XLCommandEvent> com(new XLAlertEvent(str, this, false));
		recalcEvents_.push_back(com);
	}

	void XLSession::warning(const std::string& str) const {
		std::shared_ptr<XLCommandEvent> com(new XLAlertEvent(str, this, true));
		recalcEvents_.push_back(com);
	}
}



