#ifndef xloutputevent_hpp
#define xloutputevent_hpp

#include <xlscala/xl/xlsession.hpp>
#include <xlscala/xl/xlcommandevent.hpp>
#include <xlscala/xl/xlref.hpp>

namespace xlscala {

	class XLOutputEvent : public XLCommandEvent {
	public:
		XLOutputEvent(const std::shared_ptr< Range<Scalar> >& data, bool transpose,
			const std::shared_ptr<XLRef>& xlCell, const XLSession *session)
		: data_(data), xlCell_(xlCell),
			session_(session), transpose_(transpose) {
		}

		inline bool execute(std::shared_ptr<XLRef>& xlCell) const {
			XlpOper xlpData(data_, "datas", transpose_, true);
			xlpData.setToFree(true);
			int result = Excel12(xlSet, 0, 2, &(*(xlCell_->oper())), xlpData.get());
			if (result != xlretSuccess) {
				std::cerr << "xlSet error" << std::endl;
			}
			xlCell = xlCell_;
			return false;
		}

	protected:
		std::shared_ptr< Range<Scalar> > data_;
		std::shared_ptr<XLRef> xlCell_;
		const XLSession *session_;
		bool transpose_;
	};

	class XLInplaceOutputEvent : public XLCommandEvent {
	public:
		XLInplaceOutputEvent(const std::shared_ptr< Range<Scalar> >& data, bool transpose,
			const std::shared_ptr<XLRef>& xlCell, const XLSession *session)
		: data_(data), xlCell_(xlCell),
			session_(session), transpose_(transpose) {
		}

		inline bool execute(std::shared_ptr<XLRef>& xlCell) const {
		    if  (!transpose_)
                for (Size i=0;i<data_->rows;++i) {
                    for (Size j=0;j<data_->columns;++j) {
                        XlpOper xlpData(data_->data[i*data_->columns+j], "data", false, true);
                        xlpData.setToFree(true);
                        std::shared_ptr<XLRef> xlref(new XLRef(xlCell_->row()+i+1, xlCell_->col()+j, 1, 1, xlCell_->idSheet()));
                        int result = Excel12(xlSet, 0, 2, &(*(xlref->oper())), xlpData.get());
                        if (result != xlretSuccess) {
                            std::cerr << "xlSet error" << std::endl;
                        }
                    }
            } else {
                for (Size i=0;i<data_->rows;++i) {
                    for (Size j=0;j<data_->columns;++j) {
                        XlpOper xlpData(data_->data[i*data_->columns+j], "data", false, true);
                        xlpData.setToFree(true);
                        std::shared_ptr<XLRef> xlref(new XLRef(xlCell_->row()+j+1, xlCell_->col()+i, 1, 1, xlCell_->idSheet()));
                        int result = Excel12(xlSet, 0, 2, &(*(xlref->oper())), xlpData.get());
                        if (result != xlretSuccess) {
                            std::cerr << "xlSet error" << std::endl;
                        }
                    }
                }
            }
			return false;
		}

	protected:
		std::shared_ptr< Range<Scalar> > data_;
		std::shared_ptr<XLRef> xlCell_;
		const XLSession *session_;
		bool transpose_;
	};

}

#endif
