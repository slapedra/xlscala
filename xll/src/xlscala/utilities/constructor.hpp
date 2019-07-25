#ifndef AETHER_HPP_CONSTRUCTOR
#define AETHER_HPP_CONSTRUCTOR

#include <xlscala/utilities/equal.hpp>
#include <sstream>
#include <iostream>

namespace xlscala {

   template <typename Class, typename... Args>
    inline Class constructor(Args... args) {
        return Class(args...);
    }

    template <typename Class, typename... Args>
    std::shared_ptr<Class> shared_constructor(Args... args) {
        return std::shared_ptr<Class>(new Class(args...));
    }

}
#endif
