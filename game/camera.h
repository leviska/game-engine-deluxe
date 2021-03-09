#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

struct Camera {
	glm::vec2 Pos;
	glm::vec2 Size;

	Camera() = default;
	Camera(glm::vec2 pos, glm::vec2 size);

	glm::mat4 Matrix() const;

	void UpdateFreeCamera();
};

