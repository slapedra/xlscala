#ifndef xlmessageevent_hpp
#define xlmessageevent_hpp

#include <xlscala/xl/xlsession.hpp>
#include <xlscala/xl/xlcommandevent.hpp>
#include <xlscala/xl/xlref.hpp>

namespace xlscala {

	class XLMessageEvent : public XLCommandEvent {
	public:
		XLMessageEvent(const std::string& message, const XLSession *session)
		: xlpMessage_(message, "msg") {
			xlpMessage_.setToFree(true);
		}

		inline bool execute(std::shared_ptr<XLRef>& xlCell) const {
			int result = Excel12(xlcMessage, 0, 2, TempBool12(1), xlpMessage_.get());
			if (result != xlretSuccess) {
				std::cerr << "filling message error" << std::endl;
			}
			FreeAllTempMemory();
			return false;
		}

	protected:
		XlpOper xlpMessage_;
		const XLSession *session_;
	};

}

#endif
