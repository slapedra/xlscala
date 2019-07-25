#ifndef XLOPER_HPP_PY
#define XLOPER_HPP_PY

#include <xlsdk/xlcall.h>
#include <xlsdk/framewrk.h>


namespace xlscala {

	class DLL_API Xloper {
	public:
		Xloper(bool free = true) {
			xloper_.xltype = 0;
			free_ = free;
		}

        ~Xloper() {
			if ( (xloper_.xltype) && free_)
				Excel12(xlFree, 0, 1, &xloper_);
        }

		XLOPER12 *operator&() { return &xloper_; }

		const XLOPER12 *operator->() const { return &xloper_; }

		const XLOPER12 &operator()() const { return xloper_; }

	protected:
		XLOPER12 xloper_;
		bool free_;
	};

}

#endif

