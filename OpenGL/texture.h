#pragma once

#include "image.h"

#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture {
	// mostly for debug purpose
	static uint32_t currentId;

	uint32_t id = 0;
	glm::uvec2 size;

	int type = GL_TEXTURE_2D;

	void CreateAndLoad(const void* data);
public:
	Texture() = default;
	~Texture() { Reset(); }
	Texture(Texture&& other) noexcept;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&& other) noexcept;

	void Load(glm::uvec2 size, bool rectangle = true);
	void Load(const Image& image, bool rectangle = true);
	void Load(const std::string& name, bool rectangle = true);
	void Reset();
	void Select(uint32_t num = 0) const;

	void Update(const Image& image);

	uint32_t GetId() const { return id; }
	uint32_t GetWidth() const { return size.x; }
	uint32_t GetHeight() const { return size.y; }
	glm::uvec2 GetSize() const { return size; }
	int GetType() const { return type; }
};