#include "particles.h"

#include "imgui.h"
#include "utility.h"

#include "shaders.h"
#include "glbuffers.h"

#include <random>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float GetRand(float min, float max, std::mt19937& rng) {
	std::uniform_real_distribution<float> uid(min, max);
	return uid(rng);
}

float MinMax(float min, float x, float max) {
	return std::min(max, std::max(min, x));
}

float GetNormRand(float min, float max, std::mt19937& rng) {
	std::normal_distribution<float> uid(0, 0.5);
	return MinMax(min, uid(rng) * (max - min) / 2 + (min + max) / 2, max);
}

void ParticleRender::Load(glm::vec2 pos) {
	glGenBuffers(1, &parametersBuffer);
	transform[0] = pos[0] - std::fmod(pos[0], 8.0f);
	transform[1] = pos[1] - std::fmod(pos[1], 8.0f);

	parameters.resize(10);
	Update();
}

void ParticleRender::Reset() {
	glDeleteBuffers(1, &parametersBuffer);
}

void ParticleRender::Update() {
	std::random_device rd;
	std::mt19937 rng(rd());
	for (size_t i = 0; i < parameters.size(); i++) {
		parameters[i][0] = GetNormRand(-values[0], values[0], rng);
		for (int j = 1; j < 4; j++) {
			parameters[i][j] = GetRand(-values[j], values[j], rng);
		}
	}

	Shaders().Shaders[to_ui32(ShadersId::Particle)].Select();
	Buffers().ParticleBuffer.Bind();

	glBindBuffer(GL_ARRAY_BUFFER, parametersBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * parameters.size(), &parameters[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	ObjectBuffer::Unbind();
}

void ParticleRender::GUI() {
	int count = parameters.size();

	bool needUpdate = false;

	ImGui::Begin("Parameters", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	needUpdate = ImGui::SliderInt("Count", &count, 1, 1000) || needUpdate;
	ImGui::SliderFloat("X", &transform[0], 0, 2000);
	ImGui::SliderFloat("Y", &transform[1], 0, 1000);
	ImGui::SliderFloat("W", &transform[2], 0, 100);
	ImGui::SliderFloat("H", &transform[3], 0, 100);
	ImGui::SliderFloat("Speed", &speed, 0, 10);
	ImGui::SliderInt("Coord module", &coordMod, 1, 16);
	ImGui::SliderInt("Point size", &pointSize, 1, 16);
	ImGui::End();

	ImGui::Begin("Values", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	needUpdate = ImGui::SliderFloat("A", &values[0], 0, 10) || needUpdate;
	needUpdate = ImGui::SliderFloat("B", &values[1], 0, 10) || needUpdate;
	needUpdate = ImGui::SliderFloat("C", &values[2], 0, 10) || needUpdate;
	needUpdate = ImGui::SliderFloat("D", &values[3], 0, 10) || needUpdate;
	ImGui::End();

	parameters.resize(count);

	if (needUpdate) {
		Update();
	}
}

void ParticleRender::Draw(uint32_t dt, bool gui) {
	if (gui) GUI();

	const Shader& partShader = Shaders().Shaders[to_ui32(ShadersId::Particle)];
	partShader.Select();
	Buffers().ParticleBuffer.Bind();

	time += dt * speed / 1000000.0f;
	partShader.SetFloat("Time", time);
	partShader.SetFloat("MaxX", values[0]);
	partShader.SetInt32("CoordMod", coordMod);
	
	partShader.SetVec4("Transform", transform);

	glPointSize(static_cast<float>(pointSize));
	
	glBindBuffer(GL_ARRAY_BUFFER, parametersBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * parameters.size(), &parameters[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	glDrawArraysInstanced(GL_POINTS, 0, 1, parameters.size());

	ObjectBuffer::Unbind();
}
