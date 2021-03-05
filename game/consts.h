#pragma once

#include <glm/glm.hpp>

class ConstsImpl {
public:
	glm::uvec2 CanvasSize{ 0, 0 };
	uint32_t Scale{ 0 };
	uint32_t TileSize{ 0 };

	void Load();
	void Reset();
};

const ConstsImpl& Consts();
ConstsImpl& ConstsMut();
