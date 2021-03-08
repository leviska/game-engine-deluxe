#pragma once

#include <glm/glm.hpp>

class ConstsImpl {
public:
	glm::uvec2 CanvasSize{ 0, 0 };
	uint32_t TileSize{ 0 };
	glm::uvec2 WindowSize{ 0, 0 }; // will be updated on resize
	uint32_t Scale{ 0 }; // will be updated

	void Load();
	void Reset();
};

const ConstsImpl& Consts();
ConstsImpl& ConstsMut();

void UpdateWindowSize(glm::uvec2 size);
