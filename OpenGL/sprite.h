#pragma once
#include "texture.h"
#include "image.h"
#include "resources_types.h"

#include <cstdint>

#include <glm/glm.hpp>

struct Sprite {
	glm::vec2 Pos{ 0, 0 };
	uint32_t Id{ 0 };

	ColorType Color{ 255, 255, 255, 255 };
	float Scale{ 1 };
	float Rotation{ 0 };

	bool Visible;
	
	glm::vec4 GetRect() const;
	glm::vec4 GetTextCoords() const;
	glm::vec3 GetTransform() const;
	glm::vec2 GetSize() const;
	uint32_t GetTextureId() const;

	Sprite() = default;
	Sprite(const std::string& name);
};

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
