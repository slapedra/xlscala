#ifndef AETHER_HPP_GET
#define AETHER_HPP_GET

namespace xlscala {

    namespace _ {

        template <typename ToType, typename FromType, typename Enable = void>
        struct do_get {
            typedef const ToType& get_type;
            inline static const ToType& apply(const FromType& from) {
                return *(ToType*)(&from);
            }
        };

    }

    template <typename ToType, typename FromType>
    inline const ToType& get(const FromType& from) {
        return xlscala::_::do_get<ToType, FromType>::apply(from);
    }

    template <typename ToType, typename FromType>
    inline const ToType& get(const FromType& from, const ToType& defaultto) {
        try {
            return xlscala::_::do_get<ToType, FromType>::apply(from);
        } catch (...) {
            return defaultto;
        }
    }

    template <typename ToType, typename FromType>
    inline const std::shared_ptr<ToType>& getShared(const FromType& from) {
        return xlscala::get<std::shared_ptr<ToType>, FromType>(from);
    }

    template <typename ToType, typename FromType>
    inline const std::shared_ptr<ToType>& getShared(const FromType& from, const std::shared_ptr<ToType>& defaultto) {
        try {
            return xlscala::get<std::shared_ptr<ToType>, FromType>(from);
        } catch (...) {
            return defaultto;
        }
    }

}

#endif
