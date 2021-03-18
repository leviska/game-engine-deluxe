#pragma once

#include <graphics/sprite_ptr.h>
#include <systems/tiling.h>

#include <resources/graphics.h>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include <vector>

using Renderable = std::vector<SpriteOwner>;

struct SimpleSpriteData {
	std::string Name = "Square"; 

	inline static const std::string ComponentName{ "SimpleSpriteData" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};


struct ComplexSpriteData {
	ComplexSpriteData();

	SpriteInfo Info;

	inline static const std::string ComponentName{ "ComplexSpriteData" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};


struct TilableSpriteData {
	size_t Id;

	inline static const std::string ComponentName{ "TilableSpriteData" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};
