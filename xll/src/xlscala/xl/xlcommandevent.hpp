#ifndef XLSCALA_HPP_XLCOMMAND
#define XLSCALA_HPP_XLCOMMAND

#include <xlscala/xl/xlref.hpp>

namespace xlscala {

	class XLCommandEvent {
	public:
		virtual bool execute(std::shared_ptr<XLRef>& xlCell) const = 0;
	};

}

#endif
