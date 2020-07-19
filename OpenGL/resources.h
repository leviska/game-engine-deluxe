#pragma once
#include "shader.h"
#include "texture.h"

#include "animated_sprite.h"

#include <memory>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

template<typename T>
class NamedVector {
	std::vector<T> values;
	std::unordered_map<std::string, size_t> names;

public:
	size_t Add(const T& value) {
		size_t id = values.size();
		values.push_back(value);
		return id;
	}

	size_t Add(T&& value) {
		size_t id = values.size();
		values.push_back(std::move(value));
		return id;
	}

	size_t Add(const T& value, const std::string& name) {
		size_t id = values.size();
		names[name] = id;
		values.push_back(value);
		return id;
	}

	size_t Add(T&& value, const std::string& name) {
		size_t id = values.size();
		names[name] = id;
		values.push_back(std::move(value));
		return id;
	}

	void AddName(size_t id, const std::string& name) {
		names[name] = id;
	}
	

	void Clear() {
		values.clear();
		names.clear();
	}


	size_t GetId(const std::string& name) const {
		return names.at(name);
	}

	size_t Size() const {
		return values.size();
	}


	T& operator[](size_t id) {
		return values[id];
	}

	T& operator[](std::string name) {
		return values[names.at(name)];
	}

	const T& operator[](size_t id) const {
		return values[id];
	}

	const T& operator[](std::string name) const {
		return values[names.at(name)];
	}
};

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
	Particle,
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

	ObjectBuffer SpriteBuffer;
	ObjectBuffer BatchBuffer;
	ObjectBuffer ParticleBuffer;

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

	uint32_t GetSpriteVAO() { return SpriteBuffer.VAO; }
	uint32_t GetBatchVAO() { return BatchBuffer.VAO; }
	uint32_t GetParticleVAO() { return ParticleBuffer.VAO; }
private:
	ResourcesInst() = default;
	~ResourcesInst();

	void _Load();
	void _Reset();

	void LoadSpriteInfo();
	void LoadAnimationInfo();
	void LoadSpriteVAO();
	void LoadBatchVAO();
	void LoadParticleVAO();

	void LoadMap();
};

ResourcesInst& Resources();
