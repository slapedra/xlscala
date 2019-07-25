#ifndef AETHER_HPP_CHARSTRING
#define AETHER_HPP_CHARSTRING


namespace xlscala {

    namespace _ {

        // do_char
        template <typename Type, typename Enable = void>
        struct do_char {
            typedef Type type;
        };

        template <typename Type>
        struct do_char< Type,
            typename std::enable_if< std::is_same<typename std::remove_extent<Type>::type, char>::value > ::type > {

            typedef std::string type;
        };

    }

    template <char c>
    struct static_char {
        enum { value = c };
    };

    template <char... Chars>
    struct static_string{
        static char value[sizeof...(Chars) + 1];
    };

    template <char... Chars> char static_string<Chars...>::value[sizeof...(Chars) + 1] = { Chars... };


}
#endif
