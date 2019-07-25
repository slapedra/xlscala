#ifndef AETHER_HPP_GET_TYPE_NAME
#define AETHER_HPP_GET_TYPE_NAME

#include <cxxabi.h>


namespace xlscala {

    template <typename Type>
    inline std::string get_type_name() {
        int status;
        char *realname;
        realname = abi::__cxa_demangle(typeid(Type).name(), 0, 0, &status);
        std::string ret = realname;
        delete realname;
        return ret;
    }

    inline std::string get_typeid_name(const std::type_info& info) {
        int status;
        char *realname;
        realname = abi::__cxa_demangle(info.name(), 0, 0, &status);
        std::string ret = realname;
        delete realname;
        return ret;
    }


}

#endif
