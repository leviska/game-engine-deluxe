#pragma once

#include "sprite.h"

#include "resources.h"
#include <vector>

class BatchedRender {
	uint32_t textureId = 0;

	uint32_t positionsBuffer = 0;
	uint32_t textCoordsBuffer = 0;
	uint32_t colorsBuffer = 0;
	uint32_t transformsBuffer = 0;

	std::vector<glm::vec2> positions;
	std::vector<glm::vec4> textCoords;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> transforms;

	void BindArray();
public:
	void Load(uint32_t TextureId);
	void Reset();
	
	void Clear();
	template <typename SpriteType>
	void Update(const std::vector<SpriteType>& sprites);
	void Draw();
};

template <typename SpriteType>
void BatchedRender::Update(const std::vector<SpriteType>& sprites) {
	BindArray();

	size_t curSize = sprites.size();
	positions.resize(curSize + sprites.size());
	textCoords.resize(curSize + sprites.size());
	colors.resize(curSize + sprites.size());
	transforms.resize(curSize + sprites.size());

	for (size_t i = 0; i < sprites.size(); i++) {
		positions[curSize + i] = sprites[i].Pos;
	}
	for (size_t i = 0; i < sprites.size(); i++) {
		textCoords[curSize + i] = sprites[i].GetTextCoords();
	}
	for (size_t i = 0; i < sprites.size(); i++) {
		colors[curSize + i] = sprites[i].Color;
	}
	for (size_t i = 0; i < sprites.size(); i++) {
		transforms[curSize + i] = sprites[i].GetTransform();
	}
}