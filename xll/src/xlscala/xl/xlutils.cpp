#include <xlscala/xl/xlutils.hpp>

namespace xlscala {

	DLL_API void freeOper(XLOPER12 *px) {
    	if (px->xltype & xltypeStr && px->val.str)
        		delete [] px->val.str;
	    else if (px->xltype & xltypeMulti && px->val.array.lparray) {
        	int size = px->val.array.rows * px->val.array.columns;
	        for (int i=0; i<size; ++i)
        	    if (px->val.array.lparray[i].xltype & xltypeStr
	            &&  px->val.array.lparray[i].val.str)
        	        delete [] px->val.array.lparray[i].val.str;
	        delete [] px->val.array.lparray;
			} else if (px->xltype & xltypeRef && px->val.mref.lpmref) {
				delete px->val.mref.lpmref;
			}
		delete px;
	}


    std::wstring s2ws(const std::string &str) {
        std::wstring wstrTo;
        wchar_t *wszTo = new wchar_t[str.length() + 1];
        wszTo[str.size()] = L'\0';
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
        wstrTo = wszTo;
        delete[] wszTo;
        return wstrTo;
    }

    std::string ws2s(const std::wstring& s) {
        int len;
        int slength = (int)s.length() + 1;
        len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
        char* buf = new char[len];
        WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
        std::string r(buf);
        delete[] buf;
        return r;
    }

	LPXLOPER12 TempStrStl(const std::string& str) {
        return TempStr12(s2ws(str).c_str());
	}


}



