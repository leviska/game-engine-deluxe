#pragma once

#include "texture.h"
#include "sprite.h"
#include "singleton.h"
#include "named_vector.h"

#include <nlohmann/json.hpp>

struct SpriteInfo : public Sprite {
	std::string Name;
	uint32_t TextureId{ 0 };
};

void from_json(const nlohmann::json& j, SpriteInfo& info);
void to_json(nlohmann::json& j, const SpriteInfo& info);

struct AnimationInfo {
	uint32_t SpriteId;
	uint32_t Delay; // in ms
};

class GraphicsImpl : public Singleton {
public:
	NamedVector<Texture> Textures;
	NamedVector<Texture> Textures2D;
	NamedVector<SpriteInfo> Sprites;

	void Load();
	void Reset();
private:
	~GraphicsImpl() { Reset(); }

	void LoadTextures();
	void LoadTexture(const std::string& fileName, const std::string& name);
	void LoadTexture(const Image& image, const std::string& name);
	void LoadSprites();
	void LoadSpriteInfo(uint32_t textId, const std::string& fileName, NamedVector<SpriteInfo>& res);

	void AddSquare(NamedVector<SpriteInfo>& to);

	friend GraphicsImpl& GraphicsMut();
};

const GraphicsImpl& Graphics();
GraphicsImpl& GraphicsMut();
