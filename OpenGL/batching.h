#pragma once

#include "sprite.h"

#include <vector>

class BatchedSprites {
	std::vector<Sprite> sprites;
	
	uint32_t positionsBuffer = 0;
	uint32_t textCoordsBuffer = 0;
	uint32_t colorsBuffer = 0;
	uint32_t transformsBuffer = 0;

	std::vector<glm::vec2> positions;
	std::vector<glm::vec4> textCoords;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> transforms;
public:
	void Load();
	void Reset();

	std::vector<Sprite>& Sprites() { return sprites; }
	const std::vector<Sprite>& Sprites() const { return sprites; }

	void Update();
	void Draw();

	Sprite& operator[](size_t index) { return sprites[index]; }
	const Sprite& operator[](size_t index) const { return sprites[index]; }
};