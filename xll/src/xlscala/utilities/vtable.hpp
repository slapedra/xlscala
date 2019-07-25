#ifndef AETHER_HPP_TYPE
#define AETHER_HPP_TYPE

#include <xlscala/utilities/equal.hpp>
#include <sstream>
#include <iostream>

namespace xlscala {

    namespace _ {

        // do_type
        template <typename Type>
		struct do_type {
			inline static const std::type_info& apply() {
				return typeid(Type);
			}
		};

		template <typename Type>
		struct do_equal {
			inline static bool apply(void* const* src, void** dst) {
			    Type* tmpsrc = (Type*)(*src);
			    Type* tmpdst = (Type*)(*dst);
			    return is_equal(*tmpsrc, *tmpdst);
			}
		};

        // do_clone
		template <typename Type>
		struct do_clone {
			inline static void apply(void* const* src, void** dst) {
			    *dst = new Type(**reinterpret_cast<Type* const*>(src));
			}
		};

		template <typename Type>
		struct do_placement_clone {
			inline static void apply(void* const* src, void** dst) {
			    Type* tmp = reinterpret_cast<Type*>(*dst);
			    new( tmp ) Type(*(Type*)(*src));
			}
		};

		// do_destroy
		template <typename Type>
		struct do_destroy {
			inline static void apply(void** object) {
				delete (*reinterpret_cast<Type**>(object));
			}
		};

		template <typename Type>
		struct do_placement_destroy {
			inline static void apply(void** object) {
			    reinterpret_cast<Type*>(*object)->~Type();
			}
		};

    }

}
#endif
