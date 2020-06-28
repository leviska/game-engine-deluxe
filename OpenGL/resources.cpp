#include "resources.h"

#include <fstream>
#include <stdexcept>
#include <glad/glad.h>

#include <nlohmann/json.hpp>

ResourcesInst& Resources() {
	return ResourcesInst::GetInstance();
}

ResourcesInst& ResourcesInst::GetInstance() {
	static ResourcesInst instance;
	return instance;
}

void ResourcesInst::Load() {
	GetInstance()._Load();
}

void ResourcesInst::Reset() {
	GetInstance()._Reset();
}

ResourcesInst::~ResourcesInst() {
	_Reset();
}

void ResourcesInst::_Load() {
	Shader spriteShader;
	spriteShader.Load("shaders/sprite_vertex.glsl", "shaders/sprite_fragment.glsl");
	shaders.Add(spriteShader, "SpriteShader");
	
	Texture spritesheet;
	spritesheet.Load("assets/spritesheet.png");
	textures.Add(spritesheet, "Spritesheet");

	LoadSpriteInfo();
	LoadSpriteVAO();
}

void ResourcesInst::_Reset() {
	shaders.Clear();
	textures.Clear();
	spriteInfo.Clear();

	glDeleteVertexArrays(1, &SpriteVAO);
	glDeleteBuffers(1, &SpriteVBO);
	glDeleteBuffers(1, &SpriteEBO);
}

void ResourcesInst::LoadSpriteInfo() {
	std::ifstream file("assets/spritesheet.json");
	if (!file.good()) {
		throw std::invalid_argument("Cannot open spritesheet.json file");
	}
	nlohmann::json jsoninfo;
	file >> jsoninfo;
	for (const auto& el : jsoninfo) {
		SpriteInfo info;
		info.TextureId = textures.GetId("Spritesheet");
		const auto& textureInfo = el["texture"];
		info.Position.x = textureInfo["x"].get<float>();
		info.Position.y = textureInfo["y"].get<float>();
		info.Size.x = textureInfo["width"].get<float>();
		info.Size.y = textureInfo["height"].get<float>();
		spriteInfo.Add(info, el["name"].get<std::string>());
	}
}

void ResourcesInst::LoadSpriteVAO() {
	const float SpriteVertices[] = {
		1.0f, 1.0f,  // top right
		1.0f, 0.0f,  // bottom right
		0.0f, 0.0f,  // bottom left
		0.0f, 1.0f   // top left 
	};
	const unsigned int SpriteIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenBuffers(1, &SpriteVBO);
	glGenVertexArrays(1, &SpriteVAO);
	glBindVertexArray(SpriteVAO);
	glGenBuffers(1, &SpriteEBO);

	glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertices), SpriteVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SpriteIndices), SpriteIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
