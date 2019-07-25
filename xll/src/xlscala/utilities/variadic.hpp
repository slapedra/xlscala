#ifndef AETHER_HPP_VARIADIC
#define AETHER_HPP_VARIADIC

namespace xlscala {

    // static_visitor
    template <typename Result>
    struct static_visitor {
        typedef Result result_type;
    };

    // apply_visitor
    template <typename Visitor, typename Visitable>
    inline typename Visitor::result_type apply_visitor(const Visitor& visitor, const Visitable& object) {
        return object.apply_visitor(visitor);
    };

    // vv_max
    template <typename Arg, typename... Args>
    struct vv_max;

    template <typename Arg1, typename Arg2, typename... Args>
    struct vv_max<Arg1, Arg2, Args...> {
        enum { cond = sizeof(Arg1) > sizeof(Arg2) };
        typedef typename std::conditional<cond, Arg1, Arg2>::type type_;
        typedef typename vv_max<type_, Args...>::type type;
    };

    template <typename Arg>
    struct vv_max<Arg> {
        typedef Arg type;
    };

    // vv_contains
    template <typename BaseType, typename... Types>
    struct vv_contains;

    template <typename BaseType, typename Type, typename... Types>
    struct vv_contains<BaseType, Type, Types...> {
        typedef typename std::conditional< std::is_same<BaseType, Type>::value, std::true_type::type, typename vv_contains<BaseType, Types...>::type >::type type;
    };

    template <typename BaseType, typename Type>
    struct vv_contains<BaseType, Type> {
        typedef typename std::is_same<BaseType, Type>::type type;
    };

    // vv_invrank
    template <typename BaseType, typename... Types>
    struct vv_invrank;

    template <typename BaseType, typename Type, typename... Types>
    struct vv_invrank<BaseType, Type, Types...> {
        enum { value = std::is_same<BaseType, Type>::value ? sizeof...(Types) : vv_invrank<BaseType, Types...>::value };
    };

    template <typename BaseType, typename Type>
    struct vv_invrank<BaseType, Type> {
        enum { value = std::is_same<BaseType, Type>::value ? 0 : -1 };
    };

    // vv_apply_visitor
    template <typename Visitor, typename... Types>
    struct vv_apply_visitor;

    template <typename Visitor, typename Type, typename... Types>
    struct vv_apply_visitor<Visitor, Type, Types...> {
        typedef typename Visitor::result_type result_type;

        static inline result_type apply(const Visitor& visitor, void *value, int invrank) {
            switch(invrank) {
                case sizeof...(Types):
                    return visitor(*(Type*)(value));
                default:
                    return vv_apply_visitor<Visitor, Types...>::apply(visitor, value, invrank);
            }
        }
    };

    template <typename Visitor, typename Type>
    struct vv_apply_visitor<Visitor, Type> {
        typedef typename Visitor::result_type result_type;

        static inline typename Visitor::result_type apply(const Visitor& visitor, void *value, int invrank) {
            switch(invrank) {
                case 0:
                    return visitor(*(Type*)(value));
                default:
                    FAIL("invalid invrank: " << invrank);
            }
        }
    };

    // vv_contains_convertible
    template <typename BaseType, typename... Types>
    struct vv_contains_convertible;

    template <typename BaseType, typename Type, typename... Types>
    struct vv_contains_convertible<BaseType, Type, Types...> {
        enum { value = std::is_convertible<BaseType, Type>::value };
        typedef typename std::conditional<value, Type, typename vv_contains_convertible<BaseType, Types...>::type >::type type;
    };

    template <typename BaseType, typename Type>
    struct vv_contains_convertible<BaseType, Type> {
        enum { value = std::is_convertible<BaseType, Type>::value };
        typedef typename std::conditional<value, Type, std::false_type >::type type;
    };

}
#endif
