#pragma once

#include "sprite_ptr.h"
#include "tiling.h"

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include <vector>

using Renderable = std::vector<SpriteOwner>;

struct SimpleSpriteData {
	std::string Name; 

	inline static const std::string ComponentName{ "SimpleSpriteData" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};

struct TilableSpriteData {
	TilingBitset Data;
	std::string Prefix;

	inline static const std::string ComponentName{ "TilableSpriteData" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};
