#ifndef AETHER_HPP_NULL
#define AETHER_HPP_NULL

namespace xlscala {

    namespace _ {

        template <typename Type, typename Enable = void>
        struct do_null {
            inline static Type apply() {
                return Type();
            }
        };

        template <typename Type, typename Enable = void>
        struct do_is_null {
            inline static bool apply(const Type& value) {
                return false;
            }
        };

        template <typename Type, typename Enable = void>
        struct do_is_error {
            inline static bool apply(const Type& value) {
                return false;
            }
        };

    }

    template <typename Type>
    inline Type null() {
        return xlscala::_::do_null<Type>::apply();
    }

    template <typename Type>
    inline Type null(const Type&) {
        return xlscala::_::do_null<Type>::apply();
    }

    template <typename Type>
    inline bool is_null(const Type& value) {
        return xlscala::_::do_is_null<Type>::apply(value);
    }

    template <typename Type>
    inline bool is_error(const Type& value) {
        return xlscala::_::do_is_error<Type>::apply(value);
    }

    template <typename Type>
    inline bool is_null_or_error(const Type& value) {
        return xlscala::_::do_is_error<Type>::apply(value) || xlscala::_::do_is_null<Type>::apply(value);
    }

}
#endif
