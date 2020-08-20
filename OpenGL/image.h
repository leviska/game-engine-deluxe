#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

glm::vec4 RGBA(glm::u8vec4 rgba);

class Image {
	std::vector<glm::u8vec4> image;
	glm::uvec2 size = { 0, 0 };
public:
	void Load(glm::uvec2 Size);
	void Load(std::string file);
	void Reset();

	glm::u8vec4& operator[](glm::uvec2 pos);
	const glm::u8vec4& operator[](glm::uvec2 pos) const;

	glm::uvec2 GetSize() const { return size; }

	const void* GetRawData() const { return &image[0]; }
	void* GetRawData() { return &image[0]; }
};