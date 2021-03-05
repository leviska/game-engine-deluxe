#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
	glm::vec2 Pos;
	glm::vec2 Size;

	Camera() = default;
	Camera(glm::vec2 pos, glm::vec2 size);

	glm::mat4 Matrix() const;

	void UpdateFreeCamera();
};

