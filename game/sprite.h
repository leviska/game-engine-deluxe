#pragma once
#include "texture.h"
#include "image.h"
#include "resources_types.h"

#include <cstdint>

#include <glm/glm.hpp>

struct Sprite {
	glm::vec2 Pos{ 0, 0 };
	glm::vec2 TextPos{ 0, 0 };
	glm::vec2 TextSize{ 0, 0 };
	glm::vec2 Scale{ 0, 0 };
	glm::vec4 Color{ 1, 1, 1, 1 };
	float rotation = 0;
};

/*
void DrawSprite(const Sprite& sprite);

void DrawSprite(
	glm::vec2 pos,
	uint32_t dataId,
	ColorType color = { 255, 255, 255, 255 },
	float scale = 1,
	float rotation = 0);

void DrawSprite(
	glm::vec2 pos,
	const Texture& texture,
	glm::vec2 size,
	glm::vec2 textPos,
	ColorType color = { 255, 255, 255, 255 },
	float scale = 1,
	float rotation = 0,
	Shaders shaderId = Shaders::Sprite);
*/