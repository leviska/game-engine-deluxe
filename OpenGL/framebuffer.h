#pragma once

#include "texture.h"
#include "sprite.h"
#include "shader.h"

#include <cstdint>

class FrameBuffer {
	uint32_t id = 0;

	Texture texture;
public:
	FrameBuffer() = default;
	~FrameBuffer() { Reset(); }

	FrameBuffer(FrameBuffer&& other) noexcept;
	FrameBuffer& operator=(FrameBuffer&& other) noexcept;
	
	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	void Load(glm::uvec2 size, bool rectangle = true);
	void Reset();

	void Clear(glm::vec4 color);

	void Select();
	static void SelectWindow();

	void Draw(glm::vec2 pos, float scale, Shader& shader);

	Texture& GetTexture() { return texture; }
};
