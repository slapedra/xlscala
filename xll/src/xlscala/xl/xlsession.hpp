#ifndef XLSCALA_HPP_XLSESSION
#define XLSCALA_HPP_XLSESSION

#include <xlscala/xl/xldefines.hpp>
#include <xlscala/xl/xlpoper.hpp>
#include <xlscala/xl/xlref.hpp>
#include <sstream>
#include <stdlib.h>
#include <xlscala/xl/xloper.hpp>
#include <list>
#include <set>
#include <map>
#include <boost/timer.hpp>
#include <boost/function.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <xlscala/xl/xlcommandevent.hpp>



namespace xlscala {

	/*! XLCurrentPosition
	 */
	class XLCurrentPosition {
	public:
		XLCurrentPosition(long id, Size row, Size col, const std::string& addr,
			const std::string& workbook, bool transpose);

		inline const std::string& addr() const { return addr_; }
		inline const std::string& workbook() const { return workbook_; }

		void addRow() const;
		void addCol() const;

        inline int id() const { return curIdSheet_; }
        inline int row() const { return curRow_; }
        inline int col() const { return curCol_; }

	 private:
		bool transpose_;
		mutable long curIdSheet_;
		mutable Size curRow_;
		mutable Size curCol_;
		std::string addr_;
		std::string workbook_;
	};

	/*! XLCaller
	*/
	class XLCaller {
	public:
		XLCaller ();

		inline Size row() const { return row_; }
		inline Size rows() const { return rows_; }
		inline Size col() const { return col_; }
		inline Size cols() const { return cols_; }
		inline const std::string& addr() const { return addr_; }
		inline const std::string& workbook() const { return workbook_; }
		inline long idSheet() const { return idSheet_; }

	private:
		Size row_;
		Size col_;
		Size rows_;
		Size cols_;
		long idSheet_;
		std::string addr_;
		std::string workbook_;
	};

	/*! XLFunction
	*/
	class XLFunction {
	public:

		XLFunction(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs);

		~XLFunction() {}

		inline std::wstring cName() const {return cName_;}
		inline std::wstring xlName() const {return xlName_;}
		inline std::wstring cArgs() const {return cArgs_;}
		inline std::wstring xlArgs() const {return xlArgs_;}
		inline std::wstring desc() const {return desc_;}
		inline std::wstring module() const {return moduleName_;}
		inline std::vector<std::wstring> descArgs() const {return descArgs_;}
		void registerFunction();
		void unregisterFunction();

	private:
		double regId_;
		std::wstring cName_;
		std::wstring cArgs_;
		std::wstring xlName_;
		std::wstring xlArgs_;
		std::wstring desc_;
		std::wstring moduleName_;
		std::vector<std::wstring> descArgs_;
	};

	/*! XLCommand
	*/
	class XLCommand {
	public:

		XLCommand(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs);

		~XLCommand() {}

		inline std::wstring cName() const {return cName_;}
		inline std::wstring xlName() const {return xlName_;}
		inline std::wstring cArgs() const {return cArgs_;}
		inline std::wstring xlArgs() const {return xlArgs_;}
		inline std::wstring desc() const {return desc_;}
		inline std::wstring module() const {return moduleName_;}
		inline std::vector<std::wstring> descArgs() const {return descArgs_;}
		void registerCommand();
		void unregisterCommand();

	private:
		double regId_;
		std::wstring cName_;
		std::wstring cArgs_;
		std::wstring xlName_;
		std::wstring xlArgs_;
		std::wstring desc_;
		std::wstring moduleName_;
		std::vector<std::wstring> descArgs_;
	};

	/*! XLSession
	*/
	class XLSession : public Singleton<XLSession> {
	friend class Singleton<XLSession>;
	public:
		XLSession()
		: output_(""), transpose_(false) {
		}

		~XLSession();

		void validateTrigger(OPER* xlTrigger);
		bool convertBool(OPER* xlValue, bool defaultValue);

		void init() {}

		void registerSession();
		void unregisterSession();

		std::string id() const { return id_; }
		std::string workbookPath() const;
		std::string workbookName() const;
		std::string worksheetName(bool little = true) const;

		void calculateNow() const;

		void push_back(const std::shared_ptr<XLFunction>& func);
		void push_back(const std::shared_ptr<XLCommand>& command);

		std::shared_ptr<XLRef> checkOutput(const std::string& output) const;
        std::string fillOutput(const std::shared_ptr< Range<Scalar> >& datas) const;
		std::string fillOutput(const std::shared_ptr<XLRef>& ref,
			const std::shared_ptr< Range<Scalar> >& datas, bool transpose, bool inplace) const;
        std::string fillOutput(const std::string& output,
			const std::shared_ptr< Range<Scalar> >& datas, bool transpose) const;

		void onRecalc() const;

		void freeze(bool flag, bool display = true) const;

		std::shared_ptr<XLCurrentPosition> currentPosition() const;
		std::shared_ptr<XLRef> currentRef(const XLCaller& caller, const std::string& output) const;

		inline bool transpose() { return transpose_; }

		void enterFunction(OPER* xlTrigger, OPER* xlTranspose = NULL, OPER* xlOutput = NULL) const;
		void exitFunction() const;

		void onHideProtect(bool flag) const;

		void defineName(const std::string& scalar, const std::string& name) const;
		void defineName(const std::shared_ptr<XLRef>& xlRef, const std::string& name) const;

		void message(const std::string& str) const;
		void error(const std::string& str) const;
		void warning(const std::string& str) const;

	private:
		mutable std::string worksheet_;
		std::string id_;
		mutable std::string output_;
		mutable bool transpose_;

		std::vector< std::shared_ptr<XLFunction> > functions_;
		std::vector< std::shared_ptr<XLCommand> > commands_;

		mutable std::list< std::shared_ptr<XLCommandEvent> > recalcEvents_;

		mutable bool flagCurrentPosition_;
		mutable std::shared_ptr<XLCurrentPosition> currentPosition_;
		mutable std::shared_ptr<XLRef> xlref_;
	};

	extern std::shared_ptr<XLFunction> function(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs);

	extern std::shared_ptr<XLCommand> command(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs);

	extern void registerXLFunction(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs);

	extern void registerXLCommand(const std::wstring& moduleName,
			const std::wstring& cName, const std::wstring& cArgs,
			const std::wstring& xlName, const std::wstring& xlArgs,
			const std::wstring& desc,
			const std::wstring& descArgs);

}

#endif
