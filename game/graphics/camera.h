#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct Camera {
	glm::vec2 Pos{ 0.0f, 0.0f };
	glm::vec2 Size{ 0.0f, 0.0f };

	Camera() = default;
	Camera(glm::vec2 pos, glm::vec2 size);

	glm::mat4 Matrix() const;

	void UpdateFreeCamera();
};


glm::vec2 GetSpritePos(glm::ivec2 pos);

glm::ivec2 GlobalToGamePos(glm::ivec2 pos);
glm::ivec2 GlobalToGamePos(glm::ivec2 pos, const Camera& camera);
glm::ivec2 GlobalToGridPos(glm::ivec2 pos, const Camera& camera);

glm::ivec2 GameToGlobalPos(glm::ivec2 pos);
glm::ivec2 GameToGlobalPos(glm::ivec2 pos, const Camera& camera);
glm::ivec2 GridToGlobalPos(glm::ivec2 pos, const Camera& camera);