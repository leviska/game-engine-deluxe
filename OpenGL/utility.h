#pragma once

#include <utility>
#include <functional>
#include <glm/glm.hpp>

namespace std {
	template <>
	struct hash<glm::ivec2> {
		std::size_t operator()(const glm::ivec2& k) const {
			std::size_t res = 0;
			res ^= std::hash<int>{}(k.x) + 0x9e3779b9 + (res << 6) + (res >> 2);
			res ^= std::hash<int>{}(k.y) + 0x9e3779b9 + (res << 6) + (res >> 2);
			return res;
		}
	};
}