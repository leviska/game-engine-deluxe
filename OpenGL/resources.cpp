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
	Shader particleShader;
	particleShader.Load("shaders/particle_vertex.glsl", "shaders/particle_fragment.glsl");
	shaders.Add(std::move(particleShader), "ParticleShader");

	Texture spritesheet;
	spritesheet.Load("assets/spritesheet.png");
	textures.Add(std::move(spritesheet), "Spritesheet");

	LoadSpriteInfo();
	LoadAnimationInfo();
	LoadSpriteVAO();
	LoadBatchVAO();
	LoadParticleVAO();

	LoadMap();
}

void ResourcesInst::_Reset() {
	shaders.Clear();
	textures.Clear();
	spriteInfo.Clear();

	glDeleteVertexArrays(1, &SpriteBuffer.VAO);
	glDeleteBuffers(1, &SpriteBuffer.VBO);
	glDeleteBuffers(1, &SpriteBuffer.EBO);
	glDeleteVertexArrays(1, &BatchBuffer.VAO);
	glDeleteBuffers(1, &BatchBuffer.VBO);
	glDeleteBuffers(1, &BatchBuffer.EBO);
	glDeleteVertexArrays(1, &ParticleBuffer.VAO);
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

	glGenVertexArrays(1, &SpriteBuffer.VAO);
	glBindVertexArray(SpriteBuffer.VAO);
	glGenBuffers(1, &SpriteBuffer.VBO);
	glGenBuffers(1, &SpriteBuffer.EBO);

	glBindBuffer(GL_ARRAY_BUFFER, SpriteBuffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SpriteBuffer.EBO);
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

	glGenVertexArrays(1, &BatchBuffer.VAO);
	glBindVertexArray(BatchBuffer.VAO);
	glGenBuffers(1, &BatchBuffer.VBO);
	glGenBuffers(1, &BatchBuffer.EBO);

	glBindBuffer(GL_ARRAY_BUFFER, BatchBuffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BatchBuffer.EBO);
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

void ResourcesInst::LoadParticleVAO() {
	glGenVertexArrays(1, &ParticleBuffer.VAO);
	glBindVertexArray(ParticleBuffer.VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);
	glBindVertexArray(0);
}

void ResourcesInst::LoadMap() {
	const int W = 14;
	const int H = 7;
	const char MAP[H][W+1] = {
		"##############",
		"##T########T##",
		"##############",
		"#************#",
		"#*****G******#",
		"#************#",
		"##############"
	};
	const int SCALE = 8;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			if (MAP[y][x] == 'T' || MAP[y][x] == 'G') {
				AnimatedSprite sprite;
				sprite.Load(animationInfo.GetId("Torch"));
				sprite.Scale = SCALE;
				sprite.Pos = { (x * 16 * SCALE) + 52, (y * 16 * SCALE) + 52 };
				AnimatedSprites.push_back(std::move(sprite));
			}
			Sprite sprite;
			sprite.Scale = SCALE;
			sprite.Pos = { (x * 16 * SCALE) + 52, (y * 16 * SCALE) + 52 };
			if (MAP[y][x] == '#' || MAP[y][x] == 'T')
				sprite.Load(spriteInfo.GetId("Wall"));
			else
				sprite.Load(spriteInfo.GetId("Floor"));
			Sprites.push_back(std::move(sprite));
		}
	}
}
