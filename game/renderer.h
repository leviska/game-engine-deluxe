#pragma once

#include "sprite_ptr.h"
#include "resources_types.h"

#include <memory>
#include <unordered_map>

class Renderer {
public: 
	void Load();
	void Reset();

	SpritePtr Stage(size_t textureId);
	SpritePtr Stage(const Sprite& sprite, size_t textureId);
	SpritePtr Stage(const SpriteInfo& info);
	SpritePtr Stage(const std::string& name);

	void Draw();
private:
	std::unordered_map<uint32_t, std::unique_ptr<BatchedRender>> renders;

	BatchedRender& GetRenderForTexture(size_t textureId);
};
