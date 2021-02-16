#pragma once

#include "resources_types.h"
#include "shader.h"
#include "texture.h"
#include "named_vector.h"
#include "sprite.h"
#include "animated_sprite.h"

#include <nlohmann/json.hpp>

class ResourcesInst {
	NamedVector<Shader> shaders;
	NamedVector<Texture> textures;
	NamedVector<SpriteInfo> spriteInfo;
	NamedVector<SpriteInfo> spriteInfoLE;
	NamedVector<std::vector<AnimationInfo>> animationInfo;

	std::vector<nlohmann::json> levels;

	ObjectBuffer spriteBuffer;
	ObjectBuffer batchBuffer;
	ObjectBuffer particleBuffer;
	ObjectBuffer shapeBuffer;

public:
	const glm::uvec2 CanvasSize = { 384, 216 };
	const uint32_t Scale = 8;
	const uint32_t TileSize = 16;

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
	size_t GetSpriteInfoId(const std::string& name) { return spriteInfo.GetId(name); }
	
	const NamedVector<SpriteInfo>& GetSpriteInfoLE() { return spriteInfoLE; }

	const std::vector<AnimationInfo>& GetAnimationInfo(uint32_t Id) { return animationInfo[Id]; }
	const std::vector<AnimationInfo>& GetAnimationInfo(const std::string& name) { return animationInfo[name]; }
	size_t GetAnimationInfoId(const std::string& name) { return animationInfo.GetId(name); }

	const nlohmann::json& GetLevelInfo(uint32_t Id) { return levels[Id]; }

	uint32_t GetSpriteVAO() { return spriteBuffer.VAO; }
	uint32_t GetBatchVAO() { return batchBuffer.VAO; }
	uint32_t GetParticleVAO() { return particleBuffer.VAO; }
	uint32_t GetShapeVAO() { return shapeBuffer.VAO; }
private:
	ResourcesInst() = default;
	~ResourcesInst();

	void _Load();
	void _Reset();

	void LoadShaders();
	void LoadTextures();
	void LoadSpriteInfo(NamedVector<SpriteInfo>& res, const std::string& fileName);
	void LoadAnimationInfo();
	void LoadSpriteVAO();
	void LoadBatchVAO();
	void LoadParticleVAO();
	void LoadShapeVAO();

	void LoadLevels();
};

ResourcesInst& Resources();
