#pragma once

#include <glm/glm.hpp>

using ColorChannel = unsigned char;
using ColorRGBA = glm::u8vec4;
using ColorNorm = glm::vec4;

ColorNorm RGBA(ColorRGBA rgba);
ColorRGBA ColorAlpha(ColorChannel alpha);

namespace Colors {
	inline constexpr ColorNorm Invisible{ 0.0, 0.0, 0.0, 0.0 };
	inline constexpr ColorNorm Black{ 0.0, 0.0, 0.0, 1.0 };
	inline constexpr ColorNorm White{ 0.0, 0.0, 0.0, 1.0 };
}
