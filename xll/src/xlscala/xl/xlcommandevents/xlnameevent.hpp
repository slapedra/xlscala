#ifndef xlnameevent_hpp
#define xlnameevent_hpp

#include <xlscala/xl/xlsession.hpp>
#include <xlscala/xl/xlcommandevent.hpp>
#include <xlscala/xl/xlref.hpp>

namespace xlscala {

	class XLNameEvent : public XLCommandEvent {
	public:
		XLNameEvent(const std::string& name, const std::shared_ptr<XLRef>& xlCell, const XLSession *session)
		: xlCell_(xlCell), xlpName_(name, "name") {
			xlpName_.setToFree(true);
		}

		inline bool execute(std::shared_ptr<XLRef>& xlCell) const {
			int result = Excel12(xlcDefineName, 0, 2, xlpName_.get(), &(*(xlCell_->oper())));
			if (result != xlretSuccess) {
				std::cerr << "xlcDefineName" << std::endl;
			}
			return false;
		}

	protected:
		std::shared_ptr<XLRef> xlCell_;
		XlpOper xlpName_;
		const XLSession *session_;
	};

}

#endif
