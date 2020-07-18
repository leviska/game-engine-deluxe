#pragma once
#include "texture.h"

#include <cstdint>

#include <glm/glm.hpp>

class Sprite {
public:
	uint32_t DataId = 0;
	glm::vec2 Pos;
	glm::vec4 Color = glm::vec4(1, 1, 1, 1);
	float Scale = 1;
	float Rotation = 0;
	bool Visible = false;
	
	virtual void Load(uint32_t dataId);
	virtual void Reset();

	glm::vec4 GetRect() const;
	glm::vec4 GetTextCoords() const;
	glm::vec3 GetTransform() const;
	glm::vec2 GetSize() const;

	void PrepareForDraw() const;
	void FastDraw() const;
	void ClearAfterDraw() const;

	void Draw() const;
};
