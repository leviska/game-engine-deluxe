#pragma once
#include "texture.h"

#include <cstdint>

#include <glm/glm.hpp>

class Sprite {
	uint32_t dataId = 0;
	glm::vec2 pos;
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	float rotation = 0;
	bool visible = false;
public:
	void Load(uint32_t DataId);
	void Reset();

	glm::vec4 GetRect();
	glm::vec4 GetTextCoords();
	glm::vec3 GetTransform();
	glm::vec2 GetSize();

	void PrepareForDraw();
	void FastDraw();
	void ClearAfterDraw();

	void Draw();

	void SetVisible(bool Visible) { visible = Visible; }
	bool IsVisible() { return visible; }
	void SetPos(const glm::vec2 Pos) { pos = Pos; }
	const glm::vec2& GetPos() { return pos; }
	glm::vec4 GetColor() { return color; }
	void SetColor(glm::vec4 Color) { color = Color; }
	float GetRotation() { return rotation; }
	void SetRotation(float Rotation) { rotation = Rotation; }
	uint32_t GetDataId() { return dataId; }
};