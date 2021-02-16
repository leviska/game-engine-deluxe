#pragma once

#include <glm/glm.hpp>

struct Sprite {
	glm::vec2 Pos{ 0.0, 0.0 };
	float Layer = 0.0;
	glm::vec2 TextPos{ 0.0, 0.0 };
	glm::vec2 TextSize{ 0.0, 0.0 };
	glm::vec2 Scale{ 1.0, 1.0 };
	glm::vec4 Color{ 1.0, 1.0, 1.0, 1.0 };
	float rotation = 0.0;
};

void HideSprite(Sprite& sprite);
void ShowSprite(Sprite& sprite);
void SwitchVisibility(Sprite& sprite);

/*
void DrawSprite(const Sprite& sprite, Shaders shaderId = Shaders::Sprite);
*/