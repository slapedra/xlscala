#include <sstream>
#include <xlscala/version.hpp>
#include <xlscala/defines.hpp>
#include <windows.h>
#include <xlsdk/xlcall.h>
#include <xlscala/xl/xlutils.hpp>
#include <xlscala/xl/xl.hpp>
#include <xlscala/scala/scalasession.hpp>
#include <xlscala/register/register.hpp>

using namespace xlscala;

static bool xlscala_initialized = false;
static bool xlscala_removed = false;

XL_EXPORT int xlAutoOpen() {
	try {
		if (!xlscala_initialized) {
		    InitFramework();
		    XLSession::instance();
		    Xloper xlSheet;
            Excel12(xlGetName, &xlSheet, 0);
            XlpOper xlpSheet(&xlSheet, "sheets");
            std::string name = xlpSheet;
			std::ifstream file(name + "\\..\\xlscala.ini");
			std::string host;
			file >> host;
			int port;
			file >> port;
			file.close();
            ScalaSession::instance().initConnection(host, port);
			registerIOFunctions();
			registerToolFunctions();
			registerObjectManagementFunctions();
			registerInfosFunctions();
			registerExecutionFunctions();
			XLSession::instance().registerSession();
			FreeAllTempMemory();
			xlscala_initialized = true;
		}
		return 1;
	} catch (const Error &e) {
		std::ostringstream err;
		err << "Error loading Addin: " << e.what();
		Excel12f(xlcAlert, 0, 1, TempStrStl(err.str()));
		return 0;
	} catch (...) {
		return 0;
	}
}


XL_EXPORT int xlAutoClose() {
	try {
		if ( (xlscala_initialized) && (xlscala_removed) ) {
		    XLSession::instance().unregisterSession();
			xlscala_initialized = false;
			xlscala_removed = false;
			QuitFramework();
		}
		return 1;
	} catch (const Error &e) {
			std::ostringstream err;
			err << "Error unloading XLScala Addin: " << e.what();
	    Excel12(xlcAlert, 0, 1, TempStrStl(err.str()));
    	return 0;
	} catch (...) {
        return 0;
	}
}

XL_EXPORT int xlAutoRemove() {
	try {
		xlscala_removed = true;
		return 1;
	} catch (const Error &e) {
        std::ostringstream err;
        err << "Error unloading XlScala Addin: " << e.what();
	    Excel12(xlcAlert, 0, 1, TempStrStl(err.str()));
    	return 0;
	} catch (...) {
			return 0;
	}
}

XL_EXPORT void xlAutoFree12(XLOPER12 *px) {
	freeOper(px);
}

XL_EXPORT XLOPER12 *xlAddInManagerInfo12(XLOPER12 *xlAction) {
	xlAutoOpen();
	XLOPER12 xlReturn;
	Excel12(xlCoerce, &xlReturn, 2, xlAction, TempInt12(xltypeInt));
	if (1 == xlReturn.val.w) {
		Excel12(xlFree, 0, 1, &xlReturn);
		return TempStrStl(XLSCALA_PRODUCTNAME);
  } else {
		Excel12(xlFree, 0, 1, &xlReturn);
		return TempErr12(xlerrNA);
	}
}


