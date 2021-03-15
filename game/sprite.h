#pragma once

#include <glm/glm.hpp>

class Renderer;
struct SpriteInfo;

struct Sprite {
	glm::vec2 Pos{ 0.0, 0.0 };
	float Layer = 0.0;
	glm::vec2 TextPos{ 0.0, 0.0 };
	glm::vec2 TextSize{ 0.0, 0.0 };
	glm::vec2 Scale{ 1.0, 1.0 };
	glm::vec4 Color{ 1.0, 1.0, 1.0, 1.0 };
	float Rotation = 0.0;

	Sprite() = default;
	Sprite(const SpriteInfo& sprite);

	Sprite& operator=(const SpriteInfo& sprite);
private:
	float textureIndex = 0;

	void Copy(const Sprite& sprite);

	friend class Renderer;
};

void HideSprite(Sprite& sprite);
void ShowSprite(Sprite& sprite);
void SwitchVisibility(Sprite& sprite);

/*
void DrawSprite(const Sprite& sprite, Shaders shaderId = Shaders::Sprite);
*/