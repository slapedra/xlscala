#include <xlscala/register/register.hpp>


namespace xlscala {

	void registerToolFunctions() {

		// tyArgs
		registerXLFunction(
			L"XLScala - tools",
			L"xlsArgs",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsArgs",
			L"args01, args02, args03, args04, args05, transpose, trigger",
			L"create a vector of arguments",
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"transpose\n"
			L"trigger");

		// tyBigArgs
		registerXLFunction(
			L"XLScala - tools",
			L"xlsBigArgs",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsBigArgs",
			L"args01, args02, args03, args04, args05, "
			L"args06, args07, args08, args09, args10, "
			L"args11, args12, args13, args14, args15, "
			L"args16, args17, args18, transpose, trigger",
			L"create a vector of arguments",
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"arguments\n"
			L"transpose\n"
			L"trigger");

		// tyTrigger
		registerXLFunction(
			L"XLScala - tools",
			L"xlsTrigger",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsTrigger",
			L"trigger01, trigger02, trigger03, trigger04, trigger05",
			L"force dependencies between cells",
			L"trigger\n"
			L"trigger\n"
			L"trigger\n"
			L"trigger\n"
			L"trigger");

		// tyBigTrigger
		registerXLFunction(
			L"XLScala - tools",
			L"xlsBigTrigger",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsBigTrigger",
			L"trigger01, trigger02, trigger03, trigger04, trigger05, trigger06, trigger07, trigger08, trigger09, trigger10, "
			L"trigger11, trigger12, trigger13, trigger14, trigger15, trigger16, trigger17, trigger18, trigger19, trigger20",
			L"force dependencies between cells",
			L"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger\n"
			"trigger");

		// tyVolatile
		registerXLFunction(
			L"XLScala - tools",
			L"xlsVolatile",
			XL_LPOPER+XL_LPOPER+XL_UNCALCED+XL_VOLATILE,
			L"xlsVolatile",
			L"trigger",
			L"fix a cell as volatile",
			L"trigger");

		// tyTranspose
		registerXLFunction(
			L"XLScala - tools",
			L"xlsTranspose",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsTranspose",
			L"scalars, trigger",
			L"transpose a scalars matrix",
			L"scalars\n"
			L"trigger");

		// tyT
		registerXLFunction(
			L"XLScala - tools",
			L"xlsTranspose",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsT",
			L"scalars, trigger",
			L"transpose a scalars matrix",
			L"scalars\n"
			L"trigger");

		// tyAdjustShape
		registerXLFunction(
			L"XLScala - tools",
			L"xlsAdjustShape",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsAdjustShape",
			L"scalars, rows, columns, transpose, trigger",
			L"adjust the shape of a matrix",
			L"scalars\n"
			L"rows\n"
			L"columns\n"
			L"transpose ouput\n"
			L"trigger");

		// tyReduce
		registerXLFunction(
			L"XLScala - tools",
			L"xlsReduce",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsReduce",
			L"scalars, transpose, trigger",
			L"reduce a matrix of arguments (no error, no missing in full lines or columns)",
			L"scalars\n"
			L"transpose output\n"
			L"trigger");

		// tyR
		registerXLFunction(
			L"XLScala - tools",
			L"xlsReduce",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsR",
			L"scalars, transpose, trigger",
			L"reduce a matrix of arguments (no error, no missing in full lines or columns)",
			L"scalars\n"
			L"transpose output\n"
			L"trigger");

		// tyPretty
		registerXLFunction(
			L"XLScala - tools",
			L"xlsPretty",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsPretty",
			L"scalars, transpose, trigger",
			L"show a cell matrix in a pretty way",
			L"scalars\n"
			L"transpose ooutput\n"
			L"trigger");

		// tyP
		registerXLFunction(
			L"XLScala - tools",
			L"xlsPretty",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsP",
			L"scalars, transpose, trigger",
			L"show a cell matrix in a pretty way",
			L"scalars\n"
			L"transpose ooutput\n"
			L"trigger");

		// tyFalseTrigger
		registerXLFunction(
			L"XLScala - tools",
			L"xlsFalseTrigger",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsFalseTrigger",
			L"scalar, false scalar, trigger",
			L"return false if scalar equal false scalar",
			L"scalar\nfalse scalar\ntrigger");

        registerXLFunction(
			L"XLScala - tools",
			L"xlsName",
			XL_LPOPER+XL_LPXLOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsName",
			L"scalars ref, name, trigger",
			L"define a worksheet name",
			L"scalars reference\n"
			L"name\n"
			L"trigger");

        registerXLFunction(
			L"XLScala - tools",
			L"xlsMessage",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsMessage",
			L"message, trigger",
			L"open a message dialog box",
			L"message\n"
			L"trigger");

        registerXLFunction(
			L"XLScala - tools",
			L"xlsWarning",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsWarning",
			L"message, trigger",
			L"open a warning dialog box",
			L"message\n"
			L"trigger");

        registerXLFunction(
			L"XLScala - tools",
			L"xlsError",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsError",
			L"message, trigger",
			L"open a error dialog box",
			L"message\n"
			L"trigger");

	}


}
