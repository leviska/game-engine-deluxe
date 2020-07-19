#include "particles.h"
#include "resources.h"

#include "imgui.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float GetRand(float min, float max) {
	return rand() * (max - min) / RAND_MAX + min;
}

void ParticleRender::Load() {
	glGenBuffers(1, &parametersBuffer);

	for (int i = 0; i < 10; i++) {
		parameters.push_back(glm::vec4(GetRand(-3.0f, 3.0f), GetRand(-1.0f, 1.0f), GetRand(-1.0f, 1.0f), GetRand(-3.0f, 3.0f)));
	}
	
	Resources().GetShader(Shaders::Particle).Select();
	glBindVertexArray(Resources().GetParticleVAO());

	glBindBuffer(GL_ARRAY_BUFFER, parametersBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * parameters.size(), &parameters[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
}

void ParticleRender::Reset() {
	glDeleteBuffers(1, &parametersBuffer);
}

void ParticleRender::Draw(uint32_t dt) {
	Resources().GetShader(Shaders::Particle).Select();
	glBindVertexArray(Resources().GetParticleVAO());

	time += dt * 5 / 1000000.0f;
	Resources().GetShader(Shaders::Particle).SetFloat("Time", time);

	ImGui::Begin("Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderFloat2("Position", &position[0], 0, 2000);
	ImGui::End();
	
	Resources().GetShader(Shaders::Particle).SetVec2("Position", position);

	glPointSize(8);

	glDrawArraysInstanced(GL_POINTS, 0, 1, parameters.size());

	glBindVertexArray(0);
}
