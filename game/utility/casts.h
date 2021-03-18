#pragma once

#include <cstdint>

// because static_cast<uint32_t>(t) is fucking long
template <typename T>
uint32_t to_ui32(T t) { return static_cast<uint32_t>(t); }

template <typename T>
int32_t to_i32(T t) { return static_cast<int32_t>(t); }
