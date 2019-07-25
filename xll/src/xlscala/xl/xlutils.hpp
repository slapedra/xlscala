#ifndef XLUTILS_HPP_PY
#define XLUTILS_HPP_PY

#include <xlscala/defines.hpp>
#include <xlscala/xl/xldefines.hpp>

#include <xlsdk/framewrk.h>


namespace xlscala {

	DLL_API void freeOper(XLOPER12 *px);

	template <class T>
	struct Range {

		Range() : rows(0), columns(0) {}
		Range(Size r, Size c) : rows(r), columns(c), data(r*c) {}
		Size size() { return rows*columns;}

		T operator()(Size i, Size j) {
			return data[i*columns+j];
		}

		Size rows;
		Size columns;
		std::vector<T> data;
	};

    std::wstring s2ws(const std::string &str);
    std::string ws2s(const std::wstring &str);

	LPXLOPER12 TempStrStl(const std::string& str);


}


#endif

