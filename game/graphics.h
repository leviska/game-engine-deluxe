#pragma once

#include "texture.h"
#include "sprite.h"
#include "singleton.h"
#include "named_vector.h"

#include <nlohmann/json.hpp>

struct SpriteInfo {
	std::string Name;
	uint32_t TextureId;
	Sprite Value;
};

void from_json(const nlohmann::json& j, SpriteInfo& info);

struct AnimationInfo {
	uint32_t SpriteId;
	uint32_t Delay; // in ms
};

class GraphicsImpl : public Singleton {
public:
	NamedVector<Texture> Textures;
	NamedVector<SpriteInfo> Sprites;
	NamedVector<SpriteInfo> EditorButtons;

	void Load();
	void Reset();
private:
	~GraphicsImpl() { Reset(); }

	void LoadTextures();
	void LoadSprites();
	void LoadSpriteInfo(uint32_t textId, const std::string& fileName, NamedVector<SpriteInfo>& res);

	friend GraphicsImpl& GraphicsMut();
};

const GraphicsImpl& Graphics();
GraphicsImpl& GraphicsMut();
