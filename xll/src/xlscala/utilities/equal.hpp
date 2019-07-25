#ifndef AETHER_HPP_EQUAL
#define AETHER_HPP_EQUAL

namespace xlscala {

     namespace _ {

        template <typename ToType, typename FromType, typename Enable = void>
        struct do_is_equal {
            inline static bool apply(const FromType& from, const ToType& to) {
                return false;
            }
        };

    }

    template <typename ToType, typename FromType>
    inline const bool is_equal(const FromType& from, const ToType& to) {
        return xlscala::_::do_is_equal<ToType, FromType>::apply(from, to);
    }

}

#endif
