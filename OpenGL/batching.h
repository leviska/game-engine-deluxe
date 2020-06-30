#pragma once

#include "sprite.h"

#include <vector>

class BatchedSprites {
	std::vector<Sprite> sprites;
	uint32_t textCoordsBuffer = 0;
	uint32_t modelsBuffer = 0;
public:
	void Load();
	void Reset();

	std::vector<Sprite>& Sprites() { return sprites; }
	const std::vector<Sprite>& Sprites() const { return sprites; }

	void Update();
	void Update(size_t index);
	void Draw();
};