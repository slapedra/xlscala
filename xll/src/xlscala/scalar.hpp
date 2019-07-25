#ifndef XLSCALA_HPP_SCALAR
#define XLSCALA_HPP_SCALAR

#include <vector>
#include <list>
#include <xlscala/variant.hpp>

namespace xlscala {

    typedef Variant<
        Integer, Real, std::string, bool,
        Null, Error
    > Scalar;

}

#endif

