#include <limits>
#include <algorithm>
#include <xlscala/defines.hpp>
#include <xlscala/scala/scalasession.hpp>
#include <boost/timer.hpp>
#include <xlscala/xl/xl.hpp>


namespace xlscala {

	XL_EXPORT OPER *xlsType(OPER* xlScalar, OPER* xlTrigger) {
		XL_BEGIN(xlTrigger)
		XlpOper xlpScalar(xlScalar, "scalar", false, true);
		Scalar scalar = xlpScalar.scalar();
        Scalar ret = ScalaSession::instance().getType(scalar);
		XL_END(ret)
	}

	XL_EXPORT OPER *xlsListAttr(OPER* xlScalar, OPER* xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpScalar(xlScalar, "scalar", false, false);
		Scalar scalar = xlpScalar;
		std::shared_ptr< Range<Scalar> > attrs = ScalaSession::instance().listAttr(scalar);
		XL_END(attrs)
	}

}



