#ifndef AETHER_HPP_CSV
#define AETHER_HPP_CSV

#include <boost/lexical_cast.hpp>

namespace xlscala {

     namespace _ {

        template <typename Type, typename Enable = void>
        struct do_get_csv {
            inline static Type apply(const std::string& value) {
                return boost::lexical_cast<Type>(value);
            }
        };

        template <typename Type, typename Enable = void>
        struct do_set_csv {
            inline static std::string apply(const Type& value) {
                return boost::lexical_cast<std::string>(value);
            }
        };

    }

    template <typename Type>
    inline Type get_csv(const std::string& value) {
        return xlscala::_::do_get_csv<Type>::apply(value);
    }

    template <typename Type>
    inline std::string set_csv(const Type& value) {
        return xlscala::_::do_set_csv<Type>::apply(value);
    }

}

#endif
