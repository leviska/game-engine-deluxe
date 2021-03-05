#include "shapes.h"

#include "utility.h"
#include "shaders.h"
#include "glbuffers.h"
#include "game.h"

#include <glad/glad.h>
#include <vector>

void DrawShape(const glm::vec2* data, size_t size, uint32_t mode, const ColorRGBA& color, float thickness) {
	const Shader& shapeShader = Shaders().Shaders[to_ui32(ShadersId::Shapes)];
	shapeShader.Select();
	shapeShader.SetVec4("Color", RGBA(color));
	shapeShader.UpdateProjection(Game().GetWindow().GetSize(), true);

	Buffers().EmptyBuffer.Bind();

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * size, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glLineWidth(thickness);

	glDrawArrays(mode, 0, size);

	glDeleteBuffers(1, &VBO);

	ObjectBuffer::Unbind();
}

void SegmentShape::Draw() {
	DrawShape(&Pos[0], 2, GL_LINES, Color, Thickness);
}

void LineStripShape::Draw() {
	DrawShape(&Points[0], Points.size(), GL_LINE_STRIP, Color, Thickness);
}

void RectangleShape::Draw() {
	std::array<glm::vec2, 8> vertexes;
	const std::array<glm::vec2, 5> signes{ glm::vec2{1, 1}, {1, -1}, {-1, -1}, {-1, 1}, {1, 1} };
	glm::vec2 fixedSize = Size + glm::vec2(0.5f, 0.5f);
	for (size_t i = 0; i < 4; i++) {
		vertexes[2 * i] = Center + fixedSize * signes[i];
		vertexes[2 * i + 1] = Center + fixedSize * signes[i + 1];
	}
	float halfThickness = (Thickness + 1) / 2 - 1;
	for (size_t i : { 0, 5 }) {
		vertexes[i] += glm::vec2(0, halfThickness);
	}
	for (size_t i : { 1, 4 }) {
		vertexes[i] -= glm::vec2(0, halfThickness);
	}
	for (size_t i : { 2, 7 }) {
		vertexes[i] += glm::vec2(halfThickness, 0);
	}
	for (size_t i : { 3, 6 }) {
		vertexes[i] -= glm::vec2(halfThickness, 0);
	}
	DrawShape(&vertexes[0], 8, GL_LINES, Color, Thickness);
}


void CircleShape::Draw() {
	const double PI = 3.141592653589793238462643383279;
	LineStripShape poly;
	poly.Color = Color;
	poly.Thickness = Thickness;
	size_t count = static_cast<size_t>(Radius);
	poly.Points.resize(count);
	double cnst = 2 * PI / (count - 1);
	for (size_t i = 0; i < count; i++) {
		double v = cnst * i;
		poly.Points[i] = glm::vec2{ std::cos(v), std::sin(v) } * Radius + Center;
	}
	poly.Draw();
}