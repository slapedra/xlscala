#ifndef AETHER_HPP_ERRORS
#define AETHER_HPP_ERRORS

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <xlscala/utilities/null.hpp>
#include <boost/current_function.hpp>


namespace xlscala {

	class Error : public std::exception {
	public:
        Error(const std::string& msg = "")
        : lMsg_(msg)    {
        }

        ~Error() throw() {
        }

		Error(const std::string& msg, const std::string& ffile,
            long line, const std::string& function) {
            std::ostringstream omsg;
            std::string file = ffile;
            std::string::size_type n = file.find_last_of("/\\");
            if (n != std::string::npos) {
                file = file.substr(n+1);
            }
            omsg << "(" << file << ", " << line << ") : "  << msg;
            /*#ifndef AETHER_LITTLE_ERROR_MSG
            omsg << "(" << file << ", " << line
                << ", " << function
                << ") : "  << msg;
            #else
            omsg << msg;
            #endif*/
            lMsg_ = omsg.str();
        }

        inline std::string msg() const {
            return lMsg_;
        }

		inline const char* what() const throw () {
		    return lMsg_.c_str();
		}

        inline bool operator==(const Error& error) const {
            return (lMsg_ == error.lMsg_);
        }

	private:
		std::string lMsg_;
	};

	namespace _ {
	    template <>
        struct do_is_error<Error> {
            inline static bool apply(const Error&) {
                return true;
            }
        };
	}

    #define FAIL(msg) \
	do { \
        std::ostringstream _os_fail_stream_; \
        _os_fail_stream_ << msg; \
        throw xlscala::Error(_os_fail_stream_.str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION); \
    } while(false)

    #ifdef DEBUG
    #define ASSERT(cond, msg) \
	if (!(cond)) { \
        std::ostringstream _os_assert_stream_; \
        _os_assert_stream_ << msg; \
        throw xlscala::Error(_os_assert_stream_.str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION); \
    }
    #else
    #define ASSERT(cond, msg)
    #endif

    #define REQUIRE(cond, msg) \
	if (!(cond)) { \
        std::ostringstream _os_require_stream_; \
        _os_require_stream_ << msg; \
        throw xlscala::Error(_os_require_stream_.str(), __FILE__, __LINE__, BOOST_CURRENT_FUNCTION); \
    }

    inline std::ostream &operator<<(std::ostream &out, const xlscala::Error& error) {
        out << "error: " << error.what();
        return out;

    }

}

#endif
