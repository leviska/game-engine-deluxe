#include "resources.h"

#include <fstream>
#include <stdexcept>
#include <glad/glad.h>

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
	LoadShaders();
	LoadTextures();
	
	LoadSpriteInfo(spriteInfo, "spritesheet.json");
	LoadSpriteInfo(spriteInfoLE, "spritesheet_le.json");
	LoadAnimationInfo();
	
	LoadSpriteVAO();
	LoadBatchVAO();
	LoadParticleVAO();
	LoadShapeVAO();

	LoadLevels();
}

void DeleteObjectBuffer(ObjectBuffer& buffer) {
	glDeleteVertexArrays(1, &buffer.VAO);
	glDeleteBuffers(1, &buffer.VBO);
	glDeleteBuffers(1, &buffer.EBO);

	buffer.VAO = 0;
	buffer.VBO = 0;
	buffer.EBO = 0;
}

void ResourcesInst::_Reset() {
	shaders.Clear();
	textures.Clear();
	spriteInfo.Clear();
	animationInfo.Clear();
	
	levels.clear();

	DeleteObjectBuffer(spriteBuffer);
	DeleteObjectBuffer(batchBuffer);
	DeleteObjectBuffer(particleBuffer);
	DeleteObjectBuffer(shapeBuffer);
}

void ResourcesInst::LoadShaders() {
	Shader shader;
	shader.Load("shaders/sprite_vertex.glsl", "shaders/sprite_fragment.glsl");
	shaders.Add(std::move(shader), "SpriteShader");
	shader.Load("shaders/batch_vertex.glsl", "shaders/batch_fragment.glsl");
	shaders.Add(std::move(shader), "BatchShader");
	shader.Load("shaders/shapes_vertex.glsl", "shaders/shapes_fragment.glsl");
	shaders.Add(std::move(shader), "ShapesShader");
	shader.Load("shaders/particle_vertex.glsl", "shaders/particle_fragment.glsl");
	shaders.Add(std::move(shader), "ParticleShader");
}

void ResourcesInst::LoadTextures() {
	Texture spritesheet;
	spritesheet.Load("assets/spritesheet.png");
	textures.Add(std::move(spritesheet), "Spritesheet");
	spritesheet.Load("assets/spritesheet_le.png", false);
	textures.Add(std::move(spritesheet), "Spritesheet_LE");
}

void ResourcesInst::LoadSpriteInfo(NamedVector<SpriteInfo>& res, const std::string& fileName) {
	std::ifstream file("assets/" + fileName);
	if (!file.good()) {
		throw std::invalid_argument("Cannot open " + fileName + " file");
	}
	nlohmann::json parsed;
	file >> parsed;
	const auto& frames = parsed["frames"];
	for (auto it = frames.begin(); it != frames.end(); ++it) {
		SpriteInfo info;
		info.Name = it.key();
		info.TextureId = textures.GetId("Spritesheet");
		const auto& textureInfo = it.value()["frame"];
		info.Position.x = textureInfo["x"].get<float>();
		info.Position.y = textureInfo["y"].get<float>();
		info.Size.x = textureInfo["w"].get<float>();
		info.Size.y = textureInfo["h"].get<float>();
		res.Add(std::move(info), it.key());
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
			info.Delay = el["delay"].get<uint32_t>() * 1000;
			buffer.push_back(std::move(info));
		}
		animationInfo.Add(std::move(buffer), anim["name"].get<std::string>());
	}
}

void ResourcesInst::LoadSpriteVAO() {
	const float RectVertices[] = {
		0.5f, 0.5f,  // top right
		0.5f, -0.5f,  // bottom right
		-0.5f, -0.5f,  // bottom left
		-0.5f, 0.5f   // top left 
	};
	const uint32_t RectIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &spriteBuffer.VAO);
	glBindVertexArray(spriteBuffer.VAO);
	glGenBuffers(1, &spriteBuffer.VBO);
	glGenBuffers(1, &spriteBuffer.EBO);

	glBindBuffer(GL_ARRAY_BUFFER, spriteBuffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteBuffer.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectIndices), RectIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ResourcesInst::LoadBatchVAO() {
	const float RectVertices[] = {
		0.5f, 0.5f,  // top right
		0.5f, -0.5f,  // bottom right
		-0.5f, -0.5f,  // bottom left
		-0.5f, 0.5f   // top left 
	};
	const uint32_t RectIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &batchBuffer.VAO);
	glBindVertexArray(batchBuffer.VAO);
	glGenBuffers(1, &batchBuffer.VBO);
	glGenBuffers(1, &batchBuffer.EBO);

	glBindBuffer(GL_ARRAY_BUFFER, batchBuffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchBuffer.EBO);
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
	glGenVertexArrays(1, &particleBuffer.VAO);
	glBindVertexArray(particleBuffer.VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);
	glBindVertexArray(0);
}

void ResourcesInst::LoadShapeVAO() {
	glGenVertexArrays(1, &shapeBuffer.VAO);
	glBindVertexArray(shapeBuffer.VAO);

	glBindVertexArray(0);
}

void ResourcesInst::LoadLevels() {
	std::ifstream levelsFile("assets/levels.json");
	if (!levelsFile.good()) {
		throw std::invalid_argument("Cannot open levels.json file");
	}
	nlohmann::json levelFiles;
	levelsFile >> levelFiles;
	for (const auto & levelName : levelFiles) {
		std::string fileName = levelName["Name"].get<std::string>();

		std::ifstream level(fileName);
		if (!level.good()) {
			throw std::invalid_argument("Cannot open " + fileName + " file");
		}
		nlohmann::json levelJson;
		level >> levelJson;
		levels.push_back(std::move(levelJson));
	}
}
