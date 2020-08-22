#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

using ColorChannel = unsigned char;
using Color = glm::u8vec4;

glm::vec4 RGBA(Color rgba);

Color ColorAlpha(ColorChannel alpha);

class Image {
	std::vector<Color> image;
	glm::uvec2 size = { 0, 0 };
public:
	void Load(glm::uvec2 Size, Color DefaultValue = { 0, 0, 0, 255 });
	void Load(const std::string& file);
	void Save(const std::string& file);
	void Reset();


	Color& operator[](glm::uvec2 pos);
	const Color& operator[](glm::uvec2 pos) const;

	glm::uvec2 GetSize() const { return size; }

	const void* GetRawData() const { return &image[0]; }
	void* GetRawData() { return &image[0]; }
};
