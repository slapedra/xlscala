/*
 2008 Sebastien Lapedra
*/

#ifndef xldefines_hpp
#define xldefines_hpp

#include <windows.h>
#include <xlsdk/xlcall.h>


namespace xlscala {

	#define DLL_API			    __declspec(dllexport)

	#define XL_MAX_STR_LEN		256

	#define XL_TIMESTEP			1

	// parameters registered with Excel as OPER (P) are declared as XLOPER12
	#define OPER XLOPER12

	typedef struct {
	    WORD rows;
	    WORD columns;
	    double array[1];
	} FP;

	// excel arguments type
	// generally only "P" is used
	#define XL_BOOL					std::wstring(L"A")	// short int used as logical
	#define XL_DOUBLE				std::wstring(L"B")	// double
	#define XL_CSTRING			    std::wstring(L"C")	// char * to C style NULL terminated string (up to 255 characters)
	#define XL_PSTRING			    std::wstring(L"D")	// unsigned char * to Pascal style byte counted string (up to 255 characters)
	#define XL_DOUBLE_			    std::wstring(L"E") 	// pointer to double
	#define XP_CSTRING_			    std::wstring(L"F") 	// reference to a null terminated string
	#define XL_PSTRING_			    std::wstring(L"G")	// reference to a byte counted string
	#define XL_USHORT				std::wstring(L"H")	// unsigned 2 byte int
	#define XL_SHORT				std::wstring(L"I") 	// signed 2 byte int
	#define XL_INT      		    std::wstring(L"I") 	// signed 2 byte int - platform dependent
	#define XL_LONG					std::wstring(L"J")	// signed 4 byte int
	#define XL_ARRAY				std::wstring(L"K")	// pointer to struct FP
	#define XL_BOOL_				std::wstring(L"L")	// reference to a boolean
	#define XL_SHORT_				std::wstring(L"M") 	// reference to signed 2 byte int
	#define XL_LONG_				std::wstring(L"N")	// reference to signed 4 byte int
	#define XL_ARRAY_				std::wstring(L"O")	// three arguments are ushort*, ushort*, double[]
	#define XL_LPOPER   	    	std::wstring(L"Q")	// pointer to OPER struct (never a reference type).
	#define XL_RANGE    	    	std::wstring(L"Q")	// alias for LPOPER
	#define XL_LPXLOPER		    	std::wstring(L"U")	// pointer to XLOPER12 struct
	#define XL_LPSREF				std::wstring(L"U")	// pointer to SREF struct
	#define XL_LPMREF				std::wstring(L"U")	// pointer to MREF struct

	// add in function behaviour.
	#define XL_VOLATILE			std::wstring(L"!")	// called every time sheet is recalced
	#define XL_UNCALCED 		std::wstring(L"#")	// dereferencing uncalced cells returns old value


	#define XL_BEGIN(x)\
	try {\
		xlscala::XLSession::instance().enterFunction(x);

	#define XL_BEGIN_TRANSPOSE(x, y)\
	try {\
		xlscala::XLSession::instance().enterFunction(x, y);

	#define XL_BEGIN_OUTPUT(x, y, z)\
	try {\
		xlscala::XLSession::instance().enterFunction(x, y, z);

	#define XL_END(x)\
		{\
			XlpOper _ret(x, "", XLSession::instance().transpose());\
			xlscala::XLSession::instance().exitFunction();\
			return _ret.get();\
		}\
	} catch (Error &e) { \
        ScalaSession::instance().cout(e.what()); \
		XlpOper _ret(e);\
		xlscala::XLSession::instance().exitFunction();\
		return _ret.get(); \
	} catch (...) { \
        ScalaSession::instance().cout("Unknown exception"); \
		XlpOper _ret(Error("unknown error"));\
		xlscala::XLSession::instance().exitFunction();\
		return _ret.get(); \
	}

	#define XL_END_OUTPUT(x)\
		{\
			std::string res = XLSession::instance().fillOutput(x);\
			if (res.empty()) {\
				XlpOper _ret(x, "", XLSession::instance().transpose());\
				xlscala::XLSession::instance().exitFunction();\
				return _ret.get();\
			} else {\
				XlpOper _ret(res, "");\
				xlscala::XLSession::instance().exitFunction();\
				return _ret.get();\
			}\
		}\
	} catch (Error &e) { \
		std::cerr << e.what() <<std::endl; \
		try {\
			std::shared_ptr< Range<Scalar> > datas(new Range<Scalar>(1,1)); \
			datas->data[0] = Error();\
			xlscala::XLSession::instance().fillOutput(datas); \
		} catch(Error&) {\
		}\
		XlpOper _ret(e); \
		xlscala::XLSession::instance().exitFunction();\
		return _ret.get(); \
	} catch (...) { \
		std::string msg("Unknown exception"); \
		std::cerr << msg << std::endl; \
		XlpOper _ret(Error("unknown error")); \
		xlscala::XLSession::instance().exitFunction();\
		return _ret.get(); \
	}

	#define XL_END_PRETTY(x)\
		{\
			XlpOper _ret(x, "", XLSession::instance().transpose(), true);\
			xlscala::XLSession::instance().exitFunction();\
			return _ret.get();\
		}\
	} catch (Error &e) { \
		std::cerr << e.what() <<std::endl; \
		XlpOper _ret(e);\
		xlscala::XLSession::instance().exitFunction();\
		return _ret.get(); \
	} catch (...) { \
		std::string msg("Unknown exception"); \
		std::cerr << msg << std::endl; \
		XlpOper _ret(Error("unknown error"));\
		xlscala::XLSession::instance().exitFunction();\
		return _ret.get(); \
	}

}

#endif

