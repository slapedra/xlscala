#ifndef XLPOPER_HPP_PY
#define XLPOPER_HPP_PY

#include <xlscala/defines.hpp>
#include <xlscala/xl/xlutils.hpp>
#include <vector>


namespace xlscala {

	class DLL_API XlpOper {
	public:

		XlpOper(OPER *xIn, const std::string& id, bool transpose = false,
			bool forceScalar = true, bool reduce = false);
        XlpOper(OPER *xIn, const std::string& id, const Scalar& dflt, bool transpose = false,
			bool forceScalar = true);
		XlpOper(const xlscala::Integer& i, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const Real& r, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const bool& b, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const std::string& s, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const char *c, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const Null& n, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const Scalar& s, const std::string& id = "", bool transpose = false, bool dllfree = true);
		XlpOper(const std::shared_ptr< Range<Scalar> >& sc, const std::string& id = "",
			bool transpose = false, bool pretty = false, bool dllfree = true);

		~XlpOper();

		bool missing() const;

		operator xlscala::Integer() const;
		operator Real() const;
		operator bool() const;
		operator std::string() const;
		Scalar scalar() const;
		operator Scalar() const { return scalar(); }
		operator std::shared_ptr< Range<Scalar> >() const;

		//OPER *operator->() const { return oper_; }

		OPER *get() const {
			return oper_;
		}

		void setToFree(bool flag) {
			flagToFree_ = flag;
		}

	private:
		OPER *oper_;
		std::string id_;
		bool forceScalar_;
		bool flagToFree_;

		std::shared_ptr< Range<Scalar> > scalars_;

		void convertToScalarVector(bool transpose);
		void convertToScalarVector(bool transpose, const Scalar& dflt);
		void reduceToScalarVector(bool transpose);
		void convertToOperVector(bool transpose, bool pretty, bool dllfree = true);

		bool validateMulti();
		void validateRange();

		static Scalar convertNonMultiOperToScalar(const OPER *oper);
		static Scalar convertNonMultiOperToScalar(const OPER *oper, const Scalar& dflt);
		static bool missing(const OPER *oper);
		static bool error(const OPER *oper);
		static bool scalar(const OPER *oper);

		static std::string strConv(const OPER *xString);
	};

}

#endif

