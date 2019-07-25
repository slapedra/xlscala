#ifndef AETHER_HPP_VARIANT
#define AETHER_HPP_VARIANT

#include <vector>
#include <xlscala/types.hpp>
#include <xlscala/errors.hpp>
#include <xlscala/utilities/vtable.hpp>
#include <xlscala/utilities/get.hpp>
#include <xlscala/utilities/variadic.hpp>
#include <xlscala/utilities/null.hpp>
#include <xlscala/utilities/charstring.hpp>
#include <xlscala/utilities/get_type_name.hpp>


namespace xlscala {

    namespace _ {

        struct is_variant_type {
        };

        struct variant_stream_visitor : xlscala::static_visitor<void> {
            std::ostream& out;

            variant_stream_visitor(std::ostream& out) : out(out) { }

            template <typename FromType>
            inline void operator()(FromType& from) const { out << from; }
        };
    }

    template <typename Type, typename... Types>
    class Variant {
    protected:
        typedef typename vv_max<Type, Types...>::type max_value_type;
        typedef void* void_ptr_type;

        enum {
            max_value_type_on_void_ptr_type_size = sizeof(max_value_type) / sizeof(void_ptr_type),
            modulo_size = sizeof(max_value_type) - max_value_type_on_void_ptr_type_size * sizeof(void_ptr_type),
            value_type_size = max_value_type_on_void_ptr_type_size + (modulo_size > 0 ? 1 : 0)
         };

        struct value_type{ void_ptr_type storage[value_type_size]; };

        struct vtable_type {
            mutable int invrank;
            mutable const std::type_info& (*type)();
            mutable void (*destroy)(void** object);
            mutable void (*clone)(void* const* src, void** dst);
            mutable bool (*equal)(void* const* src, void** dst);
        };

        template <typename FromType>
        struct do_vtable {
            inline static void apply(vtable_type& vtable) {
                vtable.invrank = xlscala::vv_invrank<FromType, Type, Types...>::value;
                vtable.type = xlscala::_::do_type<FromType>::apply;
                vtable.destroy = xlscala::_::do_placement_destroy<FromType>::apply;
                vtable.clone = xlscala::_::do_placement_clone<FromType>::apply;
                vtable.equal = xlscala::_::do_equal<FromType>::apply;
            }
        };

    protected:
        mutable value_type value_;
        mutable vtable_type vtable_;

    public:
        typedef _::is_variant_type is_variant_type;

    public:
        Variant() {
          do_vtable<Type>::apply(vtable_);
          new( (Type*)(&value_) ) Type(xlscala::null<Type>());
        }

        Variant(const Variant& value) {
            vtable_ = value.vtable_;
            void* tmp1 = (void*)(&value_);
            void* tmp2 = (void*)(&value.value_);
            vtable_.clone(&tmp2, &tmp1);
        }

        Variant(Variant& value) {
            vtable_ = value.vtable_;
            void* tmp1 = (void*)(&value_);
            void* tmp2 = (void*)(&value.value_);
            vtable_.clone(&tmp2, &tmp1);
        }

        Variant(Variant&& value) {
            vtable_ = value.vtable_;
            do_vtable<Null>::apply(value.vtable_);
            value_ = value.value_;
        }

        template <typename FromType>
        Variant(const FromType& value) {
            typedef typename _::do_char<FromType>::type from_type;
            static_assert(vv_contains<from_type, Type, Types...>::type::value, "unsupported base type for variant type");
            do_vtable<from_type>::apply(vtable_);
            new( (from_type*)(&value_) ) from_type(value);
        }

        ~Variant() {
            void *tmp = (void*)(&value_);
            vtable_.destroy(&tmp);
        }

        inline const Variant& operator=(const Variant& value) const {
            if (vtable_.invrank == value.vtable_.invrank) {
                void* tmpdst = (void*)(&value_);
                void* tmpsrc = (void*)(&value.value_);
                vtable_.destroy(&tmpdst);
                vtable_.clone(&tmpsrc, &tmpdst);
            } else {
                void* tmpdst = (void*)(&value_);
                void* tmpsrc = (void*)(&value.value_);
                vtable_.destroy(&tmpdst);
                vtable_ = value.vtable_;
                vtable_.clone(&tmpsrc, &tmpdst);
            }
            return *this;
        }

        inline const Variant& operator=(Variant&& value) const {
            std::swap(vtable_, value.vtable_);
            std::swap(value_, value.value_);
            return *this;
        }

        template <typename FromType>
        inline const Variant& operator=(const FromType& value) const {
            typedef typename _::do_char<FromType>::type from_type;
            static_assert(vv_contains<from_type, Type, Types...>::type::value, "unsupported base type for variant type");
            int invrank = vv_invrank<from_type, Type, Types...>::value;
            if (invrank == vtable_.invrank) {
                *(from_type*)(&value_) = value;
            } else {
                void *dtmp = (void*)(&value_);
                vtable_.destroy(&dtmp);
                new( (from_type*)(&value_) ) from_type(value);
                do_vtable<from_type>::apply(vtable_);
            }
            return *this;
        }

        inline const Variant& swap(const Variant& value) const {
            std::swap(value_, value-value_);
            std::swap(vtable_, value.vtable_);
            return *this;
        }

        template <typename ToType>
        inline const Variant& swap(const ToType& value) const {
            static_assert(vv_contains<ToType, Type, Types...>::type::value, "unsupported base type for variant type");
            int invrank = vv_invrank<ToType, Type, Types...>::value;
            REQUIRE (vtable_.invrank == invrank, vtable_.type().name() << " and " << typeid(ToType).name() << " are different type");
            std::swap(*(ToType*)(&value_), value);
            return *this;
        }

        template <typename ToType>
        inline const ToType& get() const {
            static_assert(vv_contains<ToType, Type, Types...>::type::value, "unsupported base type for variant type");
            int invrank = vv_invrank<ToType, Type, Types...>::value;
            REQUIRE (vtable_.invrank == invrank, vtable_.type().name() << " and " << typeid(ToType).name() << " are different type");
            return *(ToType*)(&value_);
        }

        template <typename ToType>
        inline operator ToType() const {
            return get<ToType>();
        }

        template <typename Visitor>
        inline typename Visitor::result_type apply_visitor(const Visitor& visitor) const {
            return vv_apply_visitor<Visitor, Type, Types...>::apply(visitor, (void*)(&value_), vtable_.invrank);
        }

        inline const std::type_info& type() const { return vtable_.type(); }

        inline bool operator==(const Variant& value) const {
            if (vtable_.invrank == value.vtable_.invrank) {
                void* tmpdst = (void*)(&value_);
                void* tmpsrc = (void*)(&value.value_);
                return vtable_.equal(&tmpsrc, &tmpdst);
            } else {
                return false;
            }
        }

        template <typename ToType>
        inline bool operator==(const ToType& value) const {
            int invrank = vv_invrank<ToType, Type, Types...>::value;
            if (vtable_.invrank == value.vtable_.invrank) {
                void* tmpdst = (void*)(&value_);
                void* tmpsrc = (void*)(&value);
                return vtable_.equal(&tmpsrc, &tmpdst);
            } else {
                return false;
            }
        }
    };

    namespace _ {

        template <typename ToType, typename FromType>
        struct do_get< ToType, FromType,
            typename std::enable_if< std::is_same<typename FromType::is_variant_type, _::is_variant_type>::value >::type > {
            inline  static const ToType& apply(const FromType& from) {
                return from.get<ToType>();
            }
        };

         template <typename Type>
        struct do_is_null< Type,
            typename std::enable_if< std::is_same<typename Type::is_variant_type, _::is_variant_type>::value >::type > {
            inline static bool apply(const Type& value) {
                return (value.type() == typeid(Null));
            }
        };

        template <typename Type>
        struct do_is_error< Type,
            typename std::enable_if< std::is_same<typename Type::is_variant_type, _::is_variant_type>::value >::type > {
            inline static bool apply(const Type& value) {
                return (value.type() == typeid(Error));
            }
        };

        template <typename ToType, typename FromType>
        struct do_is_equal<ToType, FromType,
            typename std::enable_if< std::is_same<typename FromType::is_variant_type, _::is_variant_type>::value >::type > {
            inline static bool apply(const FromType& from, const ToType& to) {
                return ( from == to );
            }
        };

        /*template <typename ToType, typename FromType>
        struct do_is_equal<ToType, FromType,
            typename std::enable_if< std::is_same<typename ToType::is_variant_type, _::is_variant_type>::value >::type > {
            inline static bool apply(const FromType& from, const ToType& to) {
                return ( to == from );
            }
        };*/

    }

    template <typename Type, typename... Types>
    inline std::ostream& operator<<(std::ostream& out, const xlscala::Variant<Type, Types...>& variant) {
        xlscala::apply_visitor(xlscala::_::variant_stream_visitor(out), variant);
        return out;
    }

}



#endif

