#ifndef AETHER_HPP_TUPLEHASH
#define AETHER_HPP_TUPLEHASH

#include <xlscala/utilities/equal.hpp>
#include <boost/functional/hash.hpp>
#include <sstream>
#include <iostream>

namespace xlscala {

    namespace _ {

        template <typename T, size_t N = 0>
        struct do_get_std_hash {};

        template <typename... T, size_t N>
        struct do_get_std_hash<std::tuple<T...>, N> {
            inline static void apply(size_t& hash, const std::tuple<T...>& t) {
                do_get_std_hash<std::tuple<T...>, N-1>::apply(hash, t);
                hash = (hash << 11) | (hash >> 21);
                hash ^= std::hash<typename std::tuple_element<N, std::tuple<T...> >::type>()(std::get<N>(t));
            }
        };

        template <typename... T>
        struct do_get_std_hash<std::tuple<T...>, 0> {
            inline static void apply(size_t& hash, const std::tuple<T...>& t) {
                hash = std::hash<typename std::tuple_element<0, std::tuple<T...> >::type>()(std::get<0>(t));
            }
        };

        template <typename T, size_t N = 0>
        struct do_get_hash {};

        template <typename... T, size_t N>
        struct do_get_hash<std::tuple<T...>, N> {
            inline static void apply(size_t& hash, const std::tuple<T...>& t) {
                do_get_hash<std::tuple<T...>, N-1>::apply(hash, t);
                hash = (hash << 11) | (hash >> 21);
                hash ^= boost::hash<typename std::tuple_element<N, std::tuple<T...> >::type>()(std::get<N>(t));
            }
        };

        template <typename... T>
        struct do_get_hash<std::tuple<T...>, 0> {
            inline static void apply(size_t& hash, const std::tuple<T...>& t) {
                hash = std::hash<typename std::tuple_element<0, std::tuple<T...> >::type>()(std::get<0>(t));
            }
        };
    }
}

namespace std {

    template <typename... T>
    struct hash<tuple<T...> >: public unary_function<tuple<T...>, size_t> {
        size_t operator()(const tuple<T...>& t) const {
            size_t hash = 0;
            xlscala::_::do_get_std_hash<tuple<T...>, tuple_size<tuple<T...> >::value-1>::apply(hash, t);
            return hash;
        }
    };

    template <>
    struct hash<tuple<> >: public unary_function<tuple<>, size_t> {
        size_t operator()(const tuple<>& ) const {
            return 0;
        }
    };
}

namespace boost {

    template <typename... T>
    struct hash<std::tuple<T...> >: public std::unary_function<std::tuple<T...>, size_t> {
        size_t operator()(const std::tuple<T...>& t) const {
            size_t hash = 0;
            xlscala::_::do_get_hash<std::tuple<T...>, std::tuple_size<std::tuple<T...> >::value-1>::apply(hash, t);
            return hash;
        }
    };

    template <>
    struct hash<std::tuple<> >: public std::unary_function<std::tuple<>, size_t> {
        size_t operator()(const std::tuple<>& ) const {
            return 0;
        }
    };
}



#endif
