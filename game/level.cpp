#include "level.h"

#include "serialize_components.h"
#include "serialization.h"
#include "tiling.h"
#include "utility.h"

#include "map_components.h"
#include "renderable.h"

#include "paths.h"
#include "graphics.h"
#include "consts.h"


void LoadLevelData(entt::registry& reg, const std::string& name) {
	nlohmann::json data = LoadJson(Paths::Levels + name + std::string(".json"));
	const nlohmann::json& entities = data["Entities"];
	LoadRegistry<LevelComponents>(entities, reg);
}

void SaveLevelData(const entt::registry& reg, const std::string& name) {
	nlohmann::json res = nlohmann::json::object();
	res["Entities"] = nlohmann::json::array();
	nlohmann::json& entities = res["Entities"];
	SaveRegistry<entt::registry, LevelComponents>(entities, reg);
	SaveJson(res, Paths::Levels + name + std::string(".json"));
}

glm::vec2 GetSpritePos(glm::ivec2 pos) {
	return glm::vec2(pos) * static_cast<float>(Consts().TileSize) + static_cast<float>(Consts().TileSize / 2);
}

void AddSprite(const std::string& name, entt::entity id, entt::registry& reg, Renderer& render) {
	SpritePtr sptr = render.Stage(Graphics().Sprites[name]);
	Renderable* rend = reg.try_get<Renderable>(id);
	if (!rend) {
		rend = &reg.emplace<Renderable>(id);
	}
	rend->emplace_back(sptr);
}

void LoadRenderer(entt::registry& reg, Renderer& render) {
	auto simpleView = reg.view<SimpleSpriteData>();
	for (auto id : simpleView) {
		auto [data] = simpleView.get(id);
		AddSprite(data.Name, id, reg, render);
	}

	auto tilingView = reg.view<TilableSpriteData>();
	for (auto id : tilingView) {
		auto [data] = tilingView.get(id);
		for (const auto& p : TilingNames) {
			if (data.Data[to_ui32(p.first)]) {
				AddSprite(data.Prefix + p.second, id, reg, render);
			}
		}
	}
}

void LoadMap(entt::registry& reg, MapView& map) {
	auto view = reg.view<GridElem>();
	for (auto id : view) {
		auto [grid] = view.get(id);
		map[grid.Pos].push_back(id);
	}
}

void UpdateGridPositions(entt::registry& reg) {
	auto view = reg.view<GridElem, Renderable>();
	for (auto id : view) {
		auto [grid, rend] = view.get(id);
		for (auto& sptr : rend) {
			sptr->Pos = GetSpritePos(grid.Pos);
		}
	}
}

void LoadLevel(const std::string& name, entt::registry& reg, Renderer& render, MapView& map) {
	LoadLevelData(reg, name);
	LoadRenderer(reg, render);
	LoadMap(reg, map);
	UpdateGridPositions(reg);
}