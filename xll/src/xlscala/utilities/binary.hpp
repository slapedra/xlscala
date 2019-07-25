#ifndef AETHER_HPP_BINARY
#define AETHER_HPP_BINARY

#include <boost/lexical_cast.hpp>

namespace xlscala {

     namespace _ {

        template <typename Type, typename Enable = void>
        struct do_get_binary {
            inline static Type apply(void *data) {
                Type value;
                new( (Type*)(&value) ) Type(*((Type*)(data)));
                return value;
            }
        };

        template <typename Type, typename Enable = void>
        struct do_set_binary {
            inline static void* apply(const Type& value) {
                return (void*)(&value);
            }
        };

        template <typename Type, typename Enable = void>
        struct do_set_size {
            inline static Size apply(const Type& value) {
                return sizeof(Type);
            }
        };

        template <>
        struct do_get_binary<std::string> {
            inline static std::string apply(void *data) {
                return std::string((char *)(data));
            }
        };

        template <>
        struct do_set_binary<std::string> {
            inline static void* apply(const std::string& value) {
                return (void*)(&value[0]);
            }
        };

        template <>
        struct do_set_size<std::string> {
            inline static Size apply(const std::string& value) {
                return value.size()+1;
            }
        };

    }

    template <typename Type>
    inline Type get_binary(void *data) {
        return xlscala::_::do_get_binary<Type>::apply(data);
    }

    template <typename Type>
    inline void* set_binary(const Type& value) {
        return xlscala::_::do_set_binary<Type>::apply(value);
    }

    template <typename Type>
    inline Size set_size(const Type& value) {
        return xlscala::_::do_set_size<Type>::apply(value);
    }

}

#endif
