#include "graphics.h"

#include "paths.h"
#include "assertion.h"

#include <fstream>

void from_json(const nlohmann::json& j, SpriteInfo& info) {
	j.at("filename").get_to(info.Name);
	const auto& frame = j.at("frame");
	frame.at("x").get_to(info.Value.TextPos.x);
	frame.at("y").get_to(info.Value.TextPos.y);
	frame.at("w").get_to(info.Value.TextSize.x);
	frame.at("h").get_to(info.Value.TextSize.y);
}

void GraphicsImpl::Load() {
	LoadTextures();
	LoadSpriteInfo("spritesheet.json");

	SpriteInfo square;
	square.Name = "Square";
	square.TextureId = Textures.GetId("Square");
	square.Value.TextPos = { 0.0f, 0.0f };
	square.Value.TextSize = { 1.0f, 1.0f };
	Sprites.Add(square, square.Name);
}

void GraphicsImpl::Reset() {
	Textures.Clear();
	Sprites.Clear();
}


void GraphicsImpl::LoadTextures() {
	Texture spritesheet;
	Image white;
	white.Load({ 1, 1 }, { 255, 255, 255, 255 });
	spritesheet.Load(white);
	Textures.Add(std::move(spritesheet), "Square");
	spritesheet.Load(Paths::Graphics + "spritesheet.png");
	Textures.Add(std::move(spritesheet), "Spritesheet");
	spritesheet.Load(Paths::Graphics + "spritesheet.png", false);
	Textures.Add(std::move(spritesheet), "SpritesheetLE");
}


void GraphicsImpl::LoadSpriteInfo(const std::string& fileName) {
	std::ifstream file(Paths::Graphics + fileName);
	if (!file.good()) {
		THROWERROR("Cannot open " + fileName + " file"); 
	}
	nlohmann::json parsed;
	file >> parsed;
	
	const auto& frames = parsed["frames"];
	for (const auto& v : frames) {
		SpriteInfo value = v.get<SpriteInfo>();
		value.TextureId = Textures.GetId("Spritesheet");
		Sprites.Add(value, value.Name);
	}
}

const GraphicsImpl& Graphics() {
	return GraphicsMut();
}

GraphicsImpl& GraphicsMut() {
	static GraphicsImpl impl;
	return impl;
}
