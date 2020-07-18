#include "resources.h"

#include <fstream>
#include <stdexcept>
#include <glad/glad.h>

// disable intellisense errors for json library
// this is only an IDE setting, compilation is not affected
#ifdef __INTELLISENSE__
#pragma diag_suppress 349
#pragma diag_suppress 79
#pragma diag_suppress 65
#pragma diag_suppress 260
#endif

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
	shaders.Add(std::move(spriteShader), "SpriteShader");
	Shader batchShader;
	batchShader.Load("shaders/batch_vertex.glsl", "shaders/batch_fragment.glsl");
	shaders.Add(std::move(batchShader), "BatchShader");

	Texture spritesheet;
	spritesheet.Load("assets/spritesheet.png");
	textures.Add(std::move(spritesheet), "Spritesheet");

	LoadSpriteInfo();
	LoadAnimationInfo();
	LoadSpriteVAO();
	LoadBatchVAO();
}

void ResourcesInst::_Reset() {
	shaders.Clear();
	textures.Clear();
	spriteInfo.Clear();

	glDeleteVertexArrays(1, &SpriteVAO);
	glDeleteBuffers(1, &SpriteVBO);
	glDeleteBuffers(1, &SpriteEBO);
	glDeleteVertexArrays(1, &BatchVAO);
	glDeleteBuffers(1, &BatchVBO);
	glDeleteBuffers(1, &BatchEBO);
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
		spriteInfo.Add(std::move(info), el["name"].get<std::string>());
	}
}

void ResourcesInst::LoadAnimationInfo() {
	std::ifstream file("assets/animations.json");
	if (!file.good()) {
		throw std::invalid_argument("Cannot open animations.json file");
	}
	nlohmann::json jsoninfo;
	file >> jsoninfo;
	for (const auto& anim : jsoninfo) {
		std::vector<AnimationInfo> buffer;
		for (const auto& el : anim["frames"]) {
			AnimationInfo info;
			info.SpriteId = spriteInfo.GetId(el["sprite"]);
			info.Delay = el["delay"].get<uint32_t>();
			buffer.push_back(std::move(info));
		}
		animationInfo.Add(std::move(buffer), anim["name"].get<std::string>());
	}
}

void ResourcesInst::LoadSpriteVAO() {
	const float RectVertices[] = {
		1.0f, 1.0f,  // top right
		1.0f, 0.0f,  // bottom right
		0.0f, 0.0f,  // bottom left
		0.0f, 1.0f   // top left 
	};
	const uint32_t RectIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &SpriteVAO);
	glBindVertexArray(SpriteVAO);
	glGenBuffers(1, &SpriteVBO);
	glGenBuffers(1, &SpriteEBO);

	glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectIndices), RectIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ResourcesInst::LoadBatchVAO() {
	const float RectVertices[] = {
		1.0f, 1.0f,  // top right
		1.0f, 0.0f,  // bottom right
		0.0f, 0.0f,  // bottom left
		0.0f, 1.0f   // top left 
	};
	const uint32_t RectIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &BatchVAO);
	glBindVertexArray(BatchVAO);
	glGenBuffers(1, &BatchVBO);
	glGenBuffers(1, &BatchEBO);

	glBindBuffer(GL_ARRAY_BUFFER, BatchVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BatchEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectIndices), RectIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);
}
