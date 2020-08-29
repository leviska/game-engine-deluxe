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
	glm::uvec2 size = { 0, 0 };

	int type = GL_TEXTURE_2D;

	void CreateAndLoad(const void* data, bool storage);
public:
	Texture() = default;
	~Texture() { Reset(); }

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Load(glm::uvec2 size, bool rectangle = true, bool storage = false);
	void Load(const Image& image, bool rectangle = true);
	void Load(const std::string& name, bool rectangle = true);
	Image Save();
	void Reset();
	void Select(uint32_t num = 0) const;

	void Update(const Image& image);

	uint32_t GetId() const { return id; }
	uint32_t GetWidth() const { return size.x; }
	uint32_t GetHeight() const { return size.y; }
	glm::uvec2 GetSize() const { return size; }
	int GetType() const { return type; }
};