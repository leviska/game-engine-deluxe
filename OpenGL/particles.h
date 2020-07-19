#pragma once

#include <glm/glm.hpp>

#include <vector>

const double PI = 3.141592653589793238462643383279;

class ParticleRender {
	std::vector<glm::vec4> parameters;

	float time = 0;
	float speed = 1;
	int coordMod = 8;
	int pointSize = 8;

	glm::vec4 values{ 3.0f, 1.0f, PI, 5.0f };

	glm::vec4 transform{ 885, 560, 10, 10 };

	uint32_t parametersBuffer;
public:
	void Load(glm::vec2 pos);
	void Reset();

	void GUI();
	void Update();

	void Draw(uint32_t dt, bool gui);
};