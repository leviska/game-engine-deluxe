#pragma once
#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture {
	uint32_t id = 0;
	uint32_t width = 0;
	uint32_t height = 0;

	int type = GL_TEXTURE_2D;

public:
	Texture() = default;
	~Texture() { Reset(); }
	Texture(Texture&& other) noexcept;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Load(const std::string& name, bool rectangle = true);
	void Reset();
	void Select() const;

	uint32_t GetId() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	glm::uvec2 GetSize() const;
};