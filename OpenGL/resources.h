#pragma once
#include "shader.h"
#include "texture.h"

#include <memory>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct SpriteInfo {
	uint32_t TextureId;
	glm::vec2 Position; // texture pos
	glm::vec2 Size; // texture size
};

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

	size_t Add(const T& value, const std::string& name) {
		size_t id = values.size();
		names[name] = id;
		values.push_back(value);
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

enum class Shaders {
	Sprite,
};

class ResourcesInst {
	NamedVector<Shader> shaders;
	NamedVector<Texture> textures;
	NamedVector<SpriteInfo> spriteInfo;

	unsigned int SpriteVAO = 0;
	unsigned int SpriteVBO = 0;
	unsigned int SpriteEBO = 0;

public:
	ResourcesInst(const ResourcesInst &) = delete;
	ResourcesInst& operator=(const ResourcesInst &) = delete;

	static ResourcesInst& GetInstance();

	static void Load();
	static void Reset();

	Shader& GetShader(Shaders Id) { return shaders[static_cast<size_t>(Id)]; }
	Shader& GetShader(uint32_t Id) { return shaders[Id]; }
	Shader& GetShader(const std::string& name) { return shaders[name]; }
	Texture& GetTexture(uint32_t Id) { return textures[Id]; }
	Texture& GetTexture(const std::string& name) { return textures[name]; }
	SpriteInfo& GetSpriteInfo(uint32_t Id) { return spriteInfo[Id]; }
	SpriteInfo& GetSpriteInfo(const std::string& name) { return spriteInfo[name]; }

	unsigned int GetSpriteVAO() { return SpriteVAO; }

private:
	ResourcesInst() = default;
	~ResourcesInst();

	void _Load();
	void _Reset();

	void LoadSpriteInfo();
	void LoadSpriteVAO();
	void LoadBatchVAO();
};

ResourcesInst& Resources();
