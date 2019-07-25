#ifndef SCALAROPERVISITOR_HPP_PY
#define SCALAROPERVISITOR_HPP_PY

#include <xlscala/defines.hpp>
#include <xlscala/xl/xldefines.hpp>


namespace xlscala {

	class ScalarToOperVisitor : public xlscala::static_visitor<void> {
	public:
		ScalarToOperVisitor(OPER *oper, bool dllfree = true) :
		oper_(oper), dllfree_(dllfree) {
		}

		void operator()(const Integer &i) const;
		void operator()(const Real &r) const;
        void operator()(const bool &b) const;
		void operator()(const std::string&) const;
		void operator()(const Null&) const;
		void operator()(const Error&) const;
	private:
		OPER *oper_;
		bool dllfree_;
	};

}


#endif

