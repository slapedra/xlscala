#include <limits>
#include <algorithm>
#include <xlscala/xl/xl.hpp>
#include <xlscala/defines.hpp>
#include <xlscala/scala/scalasession.hpp>
#include <boost/timer.hpp>


namespace xlscala {


	XL_EXPORT OPER *xlsFullIO(OPER *xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		boost::circular_buffer<std::string> lines = ScalaSession::instance().lines();
		std::shared_ptr< Range<Scalar> > scalars(new Range<Scalar>(lines.size(), 2) );
		Size offset = 0;
		for (Size i=0;i<lines.size();i++) {
			scalars->data[offset] = Integer(i);
			offset++;
			scalars->data[offset] = lines[i];
			offset++;
		}
		XL_END(scalars)
	}

	XL_EXPORT OPER *xlsIO(OPER* xlLines, OPER *xlTranspose, OPER* xlTrigger) {
		XL_BEGIN_TRANSPOSE(xlTrigger, xlTranspose)
		XlpOper xlpLines(xlLines, "lines", true, false);
		xlscala::Integer nblines = xlpLines;
		REQUIRE (nblines >0, "bad number of lines");
		boost::circular_buffer<std::string> lines = ScalaSession::instance().lines();
		std::shared_ptr< Range<Scalar> > scalars;
		if (lines.size()<= 0) {
			scalars.reset(new Range<Scalar>(1, 1) );
			scalars->data[0] = "IO empty";
		} else {
			if (Size(nblines) > Size(lines.size())) {
				nblines = lines.size();
			}
			scalars.reset(new Range<Scalar>(nblines, 2) );
			Size offset = 0;
			for (int i=nblines-1;i>=0;i--) {
				scalars->data[offset] = Integer(lines.size()-i-1);
				offset++;
				scalars->data[offset] = lines[lines.size()-i-1];
				offset++;
			}
		}
		XL_END(scalars)
	}

}



