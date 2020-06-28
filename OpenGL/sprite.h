#pragma once
#include "texture.h"

#include <cstdint>

#include <glm/glm.hpp>

class Sprite {
	uint32_t dataId = 0;
	glm::vec2 pos;
	bool visible = false;
public:
	void Load(uint32_t DataId);
	void Reset();
	
	void PrepareForDraw();
	void FastDraw();
	void ClearAfterDraw();
	
	void Draw();

	void SetVisible(bool Visible) { visible = Visible; }
	bool IsVisible() { return visible; }
	void SetPos(const glm::vec2 Pos) { pos = Pos; }
	const glm::vec2& GetPos() { return pos; }
};