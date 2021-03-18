#include <resources/graphics.h>

#include <assertion.h>

#include <resources/paths.h>

#include <fstream>

void from_json(const nlohmann::json& j, SpriteInfo& info) {
	from_json(j, static_cast<Sprite&>(info));
	j.at("Name").get_to(info.Name);
	j.at("TextureId").get_to(info.TextureId);
}

void to_json(nlohmann::json& j, const SpriteInfo& info) {
	to_json(j, static_cast<const Sprite&>(info));
	j["Name"] = info.Name;
	j["TextureId"] = info.TextureId;
}

void FromAseprite(const nlohmann::json& j, SpriteInfo& info) {
	j.at("filename").get_to(info.Name);
	const auto& frame = j.at("frame");
	frame.at("x").get_to(info.TextPos.x);
	frame.at("y").get_to(info.TextPos.y);
	frame.at("w").get_to(info.TextSize.x);
	frame.at("h").get_to(info.TextSize.y);
}

void GraphicsImpl::Load() {
	LoadTextures();
	LoadSprites();
	LoadTilingNames();
}

void GraphicsImpl::Reset() {
	Textures.Clear();
	Sprites.Clear();
}


void GraphicsImpl::AddSquare(NamedVector<SpriteInfo>& to) {
	SpriteInfo square;
	square.Name = "Square";
	square.TextureId = Textures.GetId("Square");
	square.TextPos = { 0.0f, 0.0f };
	square.TextSize = { 1.0f, 1.0f };
	square.Scale = glm::vec2{ 16.0f, 16.0f };
	to.Add(square, square.Name);
}


void GraphicsImpl::LoadTexture(const std::string& fileName, const std::string& name) {
	Texture spritesheet;

	spritesheet.Load(fileName);
	Textures.Add(std::move(spritesheet), name);

	spritesheet.Load(fileName, false);
	Textures2D.Add(std::move(spritesheet), name);
}

void GraphicsImpl::LoadTexture(const Image& image, const std::string& name) {
	Texture spritesheet;

	spritesheet.Load(image);
	Textures.Add(std::move(spritesheet), name);

	spritesheet.Load(image, false);
	Textures2D.Add(std::move(spritesheet), name);
}


void GraphicsImpl::LoadTextures() {
	DOUT() << "Loading Textures" << std::endl;

	Texture spritesheet;
	
	Image white;
	white.Load({ 1, 1 }, { 255, 255, 255, 255 });

	LoadTexture(white, "Square");
	LoadTexture(Paths::Graphics + "spritesheet.png", "Spritesheet");

	DOUT() << "Successfully loaded Textures" << std::endl;
}

void GraphicsImpl::LoadSprites() {
	DOUT() << "Loading Sprites" << std::endl;

	LoadSpriteInfo(Textures.GetId("Spritesheet"), "spritesheet.json", Sprites);

	AddSquare(Sprites);

	DOUT() << "Successfully loaded Sprites" << std::endl;
}

void GraphicsImpl::LoadSpriteInfo(uint32_t textId, const std::string& fileName, NamedVector<SpriteInfo>& res) {
	std::ifstream file(Paths::Graphics + fileName);
	if (!file.good()) {
		THROWERROR("Cannot open " + Paths::Graphics + fileName + " file");
	}
	nlohmann::json parsed;
	file >> parsed;
	
	const auto& frames = parsed["frames"];
	for (const auto& v : frames) {
		SpriteInfo value;
		FromAseprite(v, value);
		value.TextureId = textId;
		res.Add(value, value.Name);
	}
}

void GraphicsImpl::LoadTilingNames() {
	std::ifstream file(Paths::Graphics + "tile_names.json");
	if (!file.good()) {
		THROWERROR("Cannot open " + Paths::Graphics + "tile_names.json" + " file");
	}
	nlohmann::json data;
	file >> data;
	for (const auto& name : data) {
		TilingNames.push_back(name);
	}
}

const GraphicsImpl& Graphics() {
	return GraphicsMut();
}

GraphicsImpl& GraphicsMut() {
	static GraphicsImpl impl;
	return impl;
}
