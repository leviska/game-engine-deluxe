#pragma once

#include <glm/glm.hpp>

#include <vector>

class ParticleRender {
	std::vector<glm::vec4> parameters;

	float time = 0;

	glm::vec2 position{ 885, 560 };

	uint32_t parametersBuffer;
public:
	void Load();
	void Reset();

	void Draw(uint32_t dt);
};