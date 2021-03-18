#pragma once

#include <graphics/color.h>

#include <vector>
#include <string>

#include <glm/glm.hpp>

class Image {
	std::vector<ColorRGBA> image;
	glm::uvec2 size = { 0, 0 };
public:
	void Load(glm::uvec2 Size, ColorRGBA DefaultValue = { 0, 0, 0, 255 });
	void Load(const std::string& file);
	void Save(const std::string& file);
	void Reset();


	ColorRGBA& operator[](glm::uvec2 pos);
	const ColorRGBA& operator[](glm::uvec2 pos) const;

	glm::uvec2 GetSize() const { return size; }

	const void* GetRawData() const { return &image[0]; }
	void* GetRawData() { return &image[0]; }
};
