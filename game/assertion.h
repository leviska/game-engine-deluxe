#pragma once
#include <string>
#include <ostream>
#include <chrono>

// https://stackoverflow.com/a/54335644
namespace FILECutting {
    template <typename T, size_t S>
    inline constexpr size_t get_file_name_offset(const T(&str)[S], size_t i = S - 1)
    {
        return (str[i] == '/' || str[i] == '\\') ? i + 1 : (i > 0 ? get_file_name_offset(str, i - 1) : 0);
    }

    template <typename T>
    inline constexpr size_t get_file_name_offset(T(&str)[1])
    {
        return 0;
    }

    namespace utility {

        template <typename T, T v>
        struct const_expr_value
        {
            static constexpr const T value = v;
        };
    }
}

#define UTILITY_CONST_EXPR_VALUE(exp) FILECutting::utility::const_expr_value<decltype(exp), exp>::value
#define __NOPATHFILE__ &__FILE__[UTILITY_CONST_EXPR_VALUE(FILECutting::get_file_name_offset(__FILE__))]

#define THROWERROR(what) throw std::runtime_error((what) + std::string(" \n") + std::string(__NOPATHFILE__) + "(" + std::to_string(__LINE__) + ")");

// do nothing
class DummyOutput {
public:
    template<typename T>
    DummyOutput& operator<<(const T&) { return *this; }

    using ostream_manipulator = std::ostream& (*)(std::ostream&);
    DummyOutput& operator<<(ostream_manipulator) { return *this; }
};

#define FORCEOUTPUT 

std::chrono::nanoseconds TimeFromStart();
std::ostream& PrintTime(std::ostream& os, std::chrono::nanoseconds ns);

#ifdef _DEBUG
#include <iostream>
#define COUT() std::cout
#elif defined FORCEOUTPUT
#include <iostream>
#define COUT() std::cerr
#else
#define COUT() DummyOutput() 
#endif

#if defined _DEBUG || defined FORCEOUTPUT
#define DOUT() PrintTime(COUT(), TimeFromStart()) << " [" << __NOPATHFILE__ << "(" << __LINE__ << ")]: "
#else 
#define DOUT() COUT()
#endif

#define NLOUT() COUT() << std::endl