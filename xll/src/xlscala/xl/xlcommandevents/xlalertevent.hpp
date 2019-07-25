#ifndef TYPHON_HPP_XLALERTEVENT
#define TYPHON_HPP_XLALERTEVENT

#include <xlscala/xl/xlsession.hpp>
#include <xlscala/xl/xlcommandevent.hpp>
#include <xlscala/xl/xlref.hpp>

namespace xlscala {

	class XLAlertEvent : public XLCommandEvent {
	public:
		XLAlertEvent(const std::string& message, const XLSession *session, bool warning = true)
		: xlpMessage_(message, "msg"), warning_(warning) {
			xlpMessage_.setToFree(true);
		}

		inline bool execute(std::shared_ptr<XLRef>& xlCell) const {
			if (warning_) {
				int result = Excel12(xlcAlert, 0, 2, xlpMessage_.get(), TempNum12(2));
				if (result != xlretSuccess) {
					std::cerr << "filling warning message error" << std::endl;
				}
			} else {
				int result = Excel12(xlcAlert, 0, 2, xlpMessage_.get(), TempNum12(3));
				if (result != xlretSuccess) {
					std::cerr << "filling error message error" << std::endl;
				}
			}
			FreeAllTempMemory();
			return !warning_;
		}

	protected:
		XlpOper xlpMessage_;
		const XLSession *session_;
		bool warning_;
	};

}

#endif
