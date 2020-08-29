#pragma once

#include <cinttypes>
#include <string>
#include <array>

#include <glm/glm.hpp>

struct SpriteInfo {
	std::string Name;
	uint32_t TextureId;
	glm::vec2 Position; // texture pos
	glm::vec2 Size; // texture size
};

struct AnimationInfo {
	uint32_t SpriteId;
	uint32_t Delay; // in ms
};

enum class Shaders {
	Sprite,
	Batch,
	Shapes,
	Particle,
	Light,
	Obstruct,
	Buffer,
	Generate,
};

const std::array ShadersArray{
	Shaders::Sprite,
	Shaders::Batch,
	Shaders::Shapes,
	Shaders::Particle,
	Shaders::Light,
	Shaders::Obstruct,
	Shaders::Buffer,
	Shaders::Generate,
};

struct ObjectBuffer {
	uint32_t VAO = 0;
	uint32_t VBO = 0;
	uint32_t EBO = 0;
};
