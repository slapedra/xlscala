
#include <xlscala/register/register.hpp>


namespace xlscala {

	void registerIOFunctions() {

		// tyFullIO
		registerXLFunction(
			L"XLScala - io",
			L"xlsFullIO",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED+XL_VOLATILE,
			L"xlsFullIO",
			L"tranpose, trigger",
			L"return full io output",
			L"transpose output data\n"
			"trigger");

		// tyIO
		registerXLFunction(
			L"XLScala - io",
			L"xlsIO",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED+XL_VOLATILE,
			L"xlsIO",
			L"nb lines, tranpose, trigger",
			L"return io output",
			L"number of lines\n"
			L"transpose output data\n"
			L"trigger");

	}

}
