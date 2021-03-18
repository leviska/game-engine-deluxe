#include <systems/level.h>
#include <systems/tiling.h>

#include <engine/serialization.h>

#include <utility/casts.h>

#include <components/serialize.h>
#include <components/list.h>

#include <resources/paths.h>
#include <resources/graphics.h>
#include <resources/consts.h>


void LoadLevelData(entt::registry& reg, const std::string& name) {
	nlohmann::json data = LoadJson(Paths::Levels + name + std::string(".json"));
	const nlohmann::json& entities = data["Entities"];
	LoadRegistry(LevelCompList(), entities, reg);
}

void SaveLevelData(const entt::registry& reg, const std::string& name) {
	nlohmann::json res = nlohmann::json::object();
	res["Entities"] = nlohmann::json::array();
	nlohmann::json& entities = res["Entities"];
	SaveRegistry(LevelCompList(), entities, reg);
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

void UpdateRenderer(entt::registry& reg, Renderer& render) {
	auto rendView = reg.view<Renderable>();
	for (auto id : rendView) {
		reg.remove<Renderable>(id);
	}

	auto simpleView = reg.view<SimpleSpriteData>();
	for (auto id : simpleView) {
		auto [data] = simpleView.get(id);
		AddSprite(data.Name, id, reg, render);
	}

	auto complexView = reg.view<ComplexSpriteData>();
	for (auto id : complexView) {
		auto [data] = complexView.get(id);
		SpritePtr sptr = render.Stage(data.Info);
		Renderable* rend = reg.try_get<Renderable>(id);
		if (!rend) {
			rend = &reg.emplace<Renderable>(id);
		}
		rend->emplace_back(sptr);
	}

	/*auto tilingView = reg.view<TilableSpriteData>();
	for (auto id : tilingView) {
		auto [data] = tilingView.get(id);
		for (const auto& p : TilingNames) {
			if (data.Data[to_ui32(p.first)]) {
				AddSprite(data.Prefix + p.second, id, reg, render);
			}
		}
	}*/
}

void UpdateMap(entt::registry& reg, MapView& map) {
	map.clear();

	auto view = reg.view<GridElem>();
	for (auto id : view) {
		auto [grid] = view.get(id);
		map[grid.Pos].insert(id);
	}
}

bool MapValid(const entt::registry& reg, const MapView& map) {
	auto view = reg.view<const GridElem>();
	if (view) {
		for (auto id : view) {
			auto [grid] = view.get(id);
			auto it = map.find(grid.Pos);
			if (it == map.end() || it->second.find(id) == it->second.end()) {
				return false;
			}
		}
	}
	
	for (const auto& pr : map) {
		for (auto id : pr.second) {
			const GridElem* grid = reg.try_get<GridElem>(id);
			if (!grid || grid->Pos != pr.first) {
				return false;
			}
		}
	}

	return true;
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
	UpdateRenderer(reg, render);
	UpdateMap(reg, map);
	UpdateGridPositions(reg);
}
