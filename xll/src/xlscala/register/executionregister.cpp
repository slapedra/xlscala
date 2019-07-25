
#include <xlscala/register/register.hpp>


namespace xlscala {

	void registerExecutionFunctions() {

		// tySet
		registerXLFunction(
			L"XLScala - core",
			L"xlsSet",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsSet",
			L"scalar, args, values, index, default, trigger",
			L"set a scala object",
			L"scalar or scala code\n"
			"arguments\n"
			"values for inserting\n"
			"index for values (for dict type uses)\n"
			"default value for inserting\n"
			"trigger");

        // tyExec
		registerXLFunction(
			L"XLScala - core",
			L"xlsExec",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsExec",
			L"code, args, transpose, trigger",
			L"execute scala code",
			L"scala code\n"
			L"arguments\n"
			L"transpose\n"
			L"trigger");

        // tyExec
		registerXLFunction(
			L"XLScala - core",
			L"xlsExecfile",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsExecfile",
			L"filename, args, transpose, trigger",
			L"execute scala code file",
			L"filename\n"
			L"arguments\n"
			L"transpose\n"
			L"trigger");

        registerXLFunction(
			L"XLScala - core",
			L"xlsAttr",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsAttr",
			L"scalar, attribute, args, trigger",
			L"store an atttribute as object",
			L"scalar\n"
			L"attribute name\n"
			L"arguments\n"
			L"trigger");

        registerXLFunction(
			L"XLScala - core",
			L"xlsImport",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsImport",
			L"module, trigger",
			L"import a module as an object",
			L"module\n"
			L"trigger");

		registerXLFunction(
			L"XLScala - core",
			L"xlsGet",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsGet",
			L"scalar, args, output, force, transpose, trigger",
			L"get a scala object",
			L"scalar or scala code\n"
			L"arguments\n"
			L"output cell matrix id\n"
			L"force get\n"
			L"transpose output data\n"
			L"trigger");

		registerXLFunction(
			L"XLScala - core",
			L"xlsGetAttr",
			XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_LPOPER+XL_UNCALCED,
			L"xlsGetAttr",
			L"scalar, attribute, args, output, force, transpose, trigger",
			L"get an atttribute",
			L"scalar\n"
			L"attribute name\n"
			L"arguments\n"
			L"output cell matrix\n"
			L"force get\n"
			L"transpose ouput datas\n"
			L"trigger");

	}

}
