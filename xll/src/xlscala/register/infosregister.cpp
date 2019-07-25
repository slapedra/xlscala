#include <xlscala/register/register.hpp>

namespace xlscala {

	void registerInfosFunctions() {

		// tyVersion
		registerXLFunction(
			L"XLScala - infos",
			L"xlsVersion",
			XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsVersion",
			L"trigger",
			L"returns the version number of typhon",
			L"trigger");

		// tyAuthor
		registerXLFunction(
			L"XLScala - infos",
			L"xlsAuthor",
			XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsAuthor",
			L"trigger",
			L"returns the author name",
			L"trigger");

		// tyWorkbookPath
		registerXLFunction(
			L"XLScala - infos",
			L"xlsWorkbookPath",
			XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsWorkbookPath",
			L"trigger",
			L"Workbook path",
			L"trigger");

		// tyWorkbookName
		registerXLFunction(
			L"XLScala - infos",
			L"xlsWorkbookName",
			XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsWorkbookName",
			L"trigger",
			L"Workbook Name",
			L"trigger");

		// tyWoksheetName
		registerXLFunction(
			L"XLScala - infos",
			L"xlsWorksheetName",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsWorksheetName",
			L"little, trigger",
			L"Worksheet name",
			L"little format\n"
			L"trigger");

        // tyWoksheetName
		registerXLFunction(
			L"XLScala - infos",
			L"xlsConnected",
			XL_LPOPER+XL_LPOPER+XL_UNCALCED+XL_VOLATILE,
			L"xlsConnected",
			L"trigger",
			L"Connection state",
			L"trigger");

	}

}
