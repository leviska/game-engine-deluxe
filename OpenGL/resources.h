#pragma once
#include "shader.h"
#include "texture.h"
#include "named_vector.h"
#include "animated_sprite.h"

#include <glm/glm.hpp>

struct SpriteInfo {
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
	Total
};

struct ObjectBuffer {
	uint32_t VAO = 0;
	uint32_t VBO = 0;
	uint32_t EBO = 0;
};

class ResourcesInst {
	NamedVector<Shader> shaders;
	NamedVector<Texture> textures;
	NamedVector<SpriteInfo> spriteInfo;
	NamedVector<std::vector<AnimationInfo>> animationInfo;

	ObjectBuffer spriteBuffer;
	ObjectBuffer batchBuffer;
	ObjectBuffer particleBuffer;

public:
	std::vector<Sprite> Sprites;
	std::vector<AnimatedSprite> AnimatedSprites;

	ResourcesInst(const ResourcesInst &) = delete;
	ResourcesInst& operator=(const ResourcesInst &) = delete;

	static ResourcesInst& GetInstance();

	static void Load();
	static void Reset();

	Shader& GetShader(Shaders Id) { return shaders[static_cast<size_t>(Id)]; }
	Shader& GetShader(uint32_t Id) { return shaders[Id]; }
	Shader& GetShader(const std::string& name) { return shaders[name]; }
	const Texture& GetTexture(uint32_t Id) { return textures[Id]; }
	const Texture& GetTexture(const std::string& name) { return textures[name]; }
	const SpriteInfo& GetSpriteInfo(uint32_t Id) { return spriteInfo[Id]; }
	const SpriteInfo& GetSpriteInfo(const std::string& name) { return spriteInfo[name]; }
	const std::vector<AnimationInfo>& GetAnimationInfo(uint32_t Id) { return animationInfo[Id]; }
	const std::vector<AnimationInfo>& GetAnimationInfo(const std::string& name) { return animationInfo[name]; }

	uint32_t GetSpriteVAO() { return spriteBuffer.VAO; }
	uint32_t GetBatchVAO() { return batchBuffer.VAO; }
	uint32_t GetParticleVAO() { return particleBuffer.VAO; }
private:
	ResourcesInst() = default;
	~ResourcesInst();

	void _Load();
	void _Reset();

	void LoadShaders();
	void LoadTextures();
	void LoadSpriteInfo();
	void LoadAnimationInfo();
	void LoadSpriteVAO();
	void LoadBatchVAO();
	void LoadParticleVAO();

	void LoadMap();
};

ResourcesInst& Resources();
