#ifndef XLREF_HPP_PY
#define XLREF_HPP_PY

#include <windows.h>
#include <xlscala/xl/xldefines.hpp>
#include <xlsdk/xlcall.h>
#include <xlsdk/framewrk.h>
#include <xlscala/xl/xlpoper.hpp>
#include <sstream>
#include <stdlib.h>
#include <xlscala/xl/xloper.hpp>
#include <list>
#include <set>

namespace xlscala {

	class XLRef{
	public:
		XLRef(const std::shared_ptr<Xloper>& xloper);

		XLRef(Size row, Size col, Size rows, Size cols, long idSheet);

		inline Size row() const { return row_; }
		inline Size rows() const { return rows_; }
		inline Size col() const { return col_; }
		inline Size cols() const { return cols_; }
		inline long idSheet() const { return idSheet_; }
		std::shared_ptr<Xloper> oper() const { return xloper_; }

		bool inter(const std::shared_ptr<XLRef>& xlRef) const;

	private:
		Size row_;
		Size col_;
		Size rows_;
		Size cols_;
		long idSheet_;
		std::shared_ptr<Xloper> xloper_;

		bool in(Real x, Real y) const;
	};
}

#endif
