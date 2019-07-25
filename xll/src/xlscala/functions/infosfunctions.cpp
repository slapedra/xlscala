#include <xlscala/defines.hpp>
#include <xlscala/xl/xl.hpp>
#include <xlscala/version.hpp>
#include <xlscala/xl/xlsession.hpp>
#include <xlscala/scala/scalasession.hpp>



namespace xlscala {

	XL_EXPORT OPER *xlsVersion(OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XL_END(XLSCALA_STR_PRODUCTVERSION)
	}


	XL_EXPORT OPER *xlsAuthor(OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XL_END("Sebastien Lapedra")
	}

	XL_EXPORT OPER *xlsWorkbookPath(OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XL_END(XLSession::instance().workbookPath())
	}

	XL_EXPORT OPER *xlsWorkbookName(OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XL_END(XLSession::instance().workbookName())
	}

	XL_EXPORT OPER *xlsWorksheetName(OPER* xlLittle, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		bool little = XLSession::instance().convertBool(xlLittle, true);
		XL_END(XLSession::instance().worksheetName(little))
	}

	XL_EXPORT OPER *xlsConnected(OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XL_END(ScalaSession::instance().connected())
	}

}



