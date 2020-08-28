#pragma once
#include <string>

#define THROWERROR(what) throw std::runtime_error((what) + std::string(" \n") + std::string(__FILE__) + "(" + std::to_string(__LINE__) + ")");