#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string>
#include <memory>
#include <limits>
#include <vector>
#include <list>
#include <sstream>
#include <xlscala/utilities/null.hpp>
#include <xlscala/utilities/get.hpp>
#include <xlscala/utilities/vtable.hpp>

namespace xlscala {

    class Null {
    public:
        inline bool operator==(const Null&) const {
            return true;
        }
    };

    typedef int Integer;
    typedef double Real;
    typedef std::size_t Size;
    typedef Real Time;

    typedef std::pair<Real, Real> RealPair;

    #define MIN_INTEGER         ((std::numeric_limits<Integer>::min)())
    #define MAX_INTEGER         ((std::numeric_limits<Integer>::max)())
    #define MIN_REAL           -((std::numeric_limits<Real>::min)())
    #define MAX_REAL            ((std::numeric_limits<Real>::max)())
    #define MIN_POSITIVE_REAL   ((std::numeric_limits<Real>::min)())
    #define EPSILON             ((std::numeric_limits<Real>::epsilon)())
    #define TIME_EPSILON        ((std::numeric_limits<Time>::epsilon)())
    #define MIN_SIZE            ((std::numeric_limits<Size>::min)())
    #define MAX_SIZE            ((std::numeric_limits<Size>::max)())
    #define NULL_INTEGER        ((std::numeric_limits<Integer>::max)())
    #define NULL_REAL           ((std::numeric_limits<Real>::max)())
    #define NILL_REAL           ((std::numeric_limits<Real>::min)())
    #define NULL_SIZE           ((std::numeric_limits<Size>::max)())

    namespace _ {

        template <>
        struct do_null<Integer> {
            inline static Integer apply() {
                return NULL_INTEGER;
            }
        };

        template <>
        struct do_null<Real> {
           inline  static Real apply() {
                return NULL_REAL;
            }
        };

        template <>
        struct do_null<Size> {
            inline static Size apply() {
                return NULL_SIZE;
            }
        };

        template <>
        struct do_is_null<Null> {
            inline static bool apply(const Null&) {
                return true;
            }
        };

        template <>
        struct do_is_null<std::string> {
            inline static bool apply(const std::string& str) {
                return str.empty();
            }
        };

		template <>
		struct do_destroy<Null> {
			inline static void apply(void** object) {
			}
		};

		template <>
		struct do_clone<Null> {
			inline static void apply(void* const* src, void** dst) {
			}
		};

		template <>
        struct do_is_equal<Null, Null> {
            inline static bool apply(const Null& from, const Null& to) {
                return true;
            }
        };

        template <>
        struct do_is_equal<Real, Real> {
            inline static bool apply(const Real& from, const Real& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<Integer, Integer> {
            inline static bool apply(const Integer& from, const Integer& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<bool, bool> {
            inline static bool apply(const bool& from, const bool& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<std::string, std::string> {
            inline static bool apply(const std::string& from, const std::string& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<Real, Integer> {
            inline static bool apply(const Integer& from, const Real& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<Integer, Real> {
            inline static bool apply(const Real& from, const Integer& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<Real, bool> {
            inline static bool apply(const bool& from, const Real& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<bool, Real> {
            inline static bool apply(const Real& from, const bool& to) {
                return ( from == to);
            }
        };


        template <>
        struct do_is_equal<bool, Integer> {
            inline static bool apply(const Integer& from, const bool& to) {
                return ( from == to);
            }
        };

        template <>
        struct do_is_equal<Integer, bool> {
            inline static bool apply(const bool& from, const Integer& to) {
                return ( from == to);
            }
        };

    }

    inline std::ostream &operator<<(std::ostream &out, const Null &) { return out << "null"; }

    template <typename Type>
	inline std::ostream& operator<<(std::ostream &out, const std::vector<Type>& values) {
	    out << "[ ";
	    for (auto iter=values.begin();iter!=(--values.end());++iter) {
	        out << *iter << ", ";
	    }
	    out << *(--values.end()) << " ]";
	    return out;
    }

    template <typename Type>
	inline std::ostream& operator<<(std::ostream &out, const std::list<Type>& values) {
	    out << "{ ";
	    for (auto iter=values.begin();iter!=(--values.end());++iter) {
	        out << *iter << ", ";
	    }
	    out << *(--values.end()) << " }";
	    return out;
    }

}

#endif // TYPES_H_INCLUDED
