#pragma once

#include <array>
#include <vector>
#include <glm/glm.hpp>

class ShapeBase {
public:
	float Thickness = 1;
	glm::vec4 Color{ 0.0f, 0.0f, 0.0f, 1.0f };
};

class SegmentShape : public ShapeBase {
public:
	std::array<glm::vec2, 2> Pos;
	
	void Draw();
};

class LineStripShape : public ShapeBase {
public:
	std::vector<glm::vec2> Points;

	void Draw();
};

class RectangleShape : public ShapeBase {
public:
	glm::vec2 Center;
	glm::vec2 Size;

	void Draw();
};

class CircleShape : public ShapeBase {
public:
	glm::vec2 Center;
	float Radius;

	void Draw();
};
