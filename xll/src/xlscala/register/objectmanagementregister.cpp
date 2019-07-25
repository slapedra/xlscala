
#include <xlscala/register/register.hpp>


namespace xlscala {

	void registerObjectManagementFunctions() {

		// tyType
		registerXLFunction(
			L"XLScala - objects",
			L"xlsType",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsType",
			L"scalar, trigger",
			L"return the type of a scalar or an object",
			L"scalar \n trigger");

        registerXLFunction(
			L"XLScala - objects",
			L"xlsListAttr",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsListAttr",
			L"scalar, transpose, trigger",
			L"list all the attributes of a scalar/object",
			L"scalar\n"
			L"transpose output data\n"
			L"trigger");
	}

}
