#pragma once
#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture {
	uint32_t id = 0;
	glm::uvec2 size;

	int type = GL_TEXTURE_2D;

	void CreateAndLoad(void* data, int mode);

public:
	Texture() = default;
	~Texture() { Reset(); }
	Texture(Texture&& other) noexcept;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&& other);

	void Load(glm::uvec2 size, bool rectangle = true);
	void Load(const std::string& name, bool rectangle = true);
	void Reset();
	void Select(uint32_t num = 0) const;

	uint32_t GetId() const { return id; }
	uint32_t GetWidth() const { return size.x; }
	uint32_t GetHeight() const { return size.y; }
	glm::uvec2 GetSize() const { return size; }
	int GetType() const { return type; }
};