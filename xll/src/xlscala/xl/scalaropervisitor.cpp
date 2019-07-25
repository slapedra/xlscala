#include <xlscala/xl/scalaropervisitor.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <xlscala/xl/xlutils.hpp>


namespace xlscala {

	#define MIN(X, Y)  ((X) < (Y) ? (X) : (Y))

	std::string toExcelString(const std::string& str) {
		return str;
	}

	/************************************************************************
		ScalarToOperVisitor
	************************************************************************/
	void ScalarToOperVisitor::operator()(const Integer &i) const {
		oper_->xltype = xltypeNum;
		/*if (dllfree_)
			oper_->xltype |= xlbitDLLFree;*/
        oper_->val.num = i;
	}

	void  ScalarToOperVisitor::operator()(const Real &r) const {
	    if (r != r) {
	        oper_->xltype = xltypeMissing;
            //if (dllfree_)
			//oper_->xltype |= xlbitDLLFree;
		} else {
		    oper_->xltype = xltypeNum;
            /*if (dllfree_)
                oper_->xltype |= xlbitDLLFree;*/
            oper_->val.num = r;
		}
	}

	void ScalarToOperVisitor::operator()(const bool &b) const {
		oper_->xltype = xltypeBool;
		//if (dllfree_)
			//oper_->xltype |= xlbitDLLFree;
        oper_->val.xbool = b;
	}

	void ScalarToOperVisitor::operator()(const std::string& sss) const {
		std::string ss = toExcelString(sss);
		std::wstring s = s2ws(ss);
		Size len = Size(MIN(XL_MAX_STR_LEN - 1, s.length()));
		oper_->val.str = new wchar_t[len + 1];
		oper_->xltype = xltypeStr;
		if (dllfree_)
			oper_->xltype |= xlbitDLLFree;
		oper_->val.str[0] = len;
		if (len) {
			std::copy(s.c_str(), s.c_str()+len, oper_->val.str + 1);
		}
	}

	void ScalarToOperVisitor::operator()(const Null&) const {
		//oper_->xltype = xltypeMissing;
		oper_->xltype = xltypeErr;
		oper_->val.err = xlerrNA;
		//if (dllfree_)
			//oper_->xltype |= xlbitDLLFree;
	}

	void ScalarToOperVisitor::operator()(const Error&) const {
		oper_->xltype = xltypeErr;
		oper_->val.err = xlerrNA;
		//if (dllfree_)
			//oper_->xltype |= xlbitDLLFree;
	}




}



