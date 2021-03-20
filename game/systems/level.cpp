#include <systems/level.h>
#include <systems/tiling.h>

#include <engine/serialization.h>

#include <utility/casts.h>

#include <components/serialize.h>
#include <components/movable.h>
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

SpritePtr AddSprite(const SpriteInfo& info, entt::entity id, entt::registry& reg, Renderer& render) {
	reg.emplace_or_replace<UpdateRendPositionFlag>(id);

	SpritePtr sptr = render.Stage(info);
	Renderable* rend = reg.try_get<Renderable>(id);
	if (!rend) {
		rend = &reg.emplace<Renderable>(id);
	}
	rend->emplace_back(sptr);
	return sptr;
}

void UpdateRenderer(entt::registry& reg, Renderer& render) {
	reg.clear<Renderable>();

	auto simpleView = reg.view<SimpleSpriteData>();
	for (auto id : simpleView) {
		auto [data] = simpleView.get(id);
		SpritePtr ptr = AddSprite(Graphics().Sprites[data.Name], id, reg, render);
		ptr->Layer = data.Layer;
	}

	auto complexView = reg.view<ComplexSpriteData>();
	for (auto id : complexView) {
		auto [data] = complexView.get(id);
		AddSprite(data.Info, id, reg, render);
	}

	auto tilingView = reg.view<TilableInfo, TilingData, TilableRender>();
	const auto& tilingNames = Graphics().TilingNames;
	for (auto id : tilingView) {
		auto [info, data] = tilingView.get(id);
		for (const auto& p : TilingNames) {
			if (data.Data[to_ui32(p.first)] && info.Id < tilingNames.size()) {
				AddSprite(Graphics().Sprites[tilingNames[info.Id] + p.second], id, reg, render);
			}
		}
	}
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

void UpdateRenderPositions(entt::registry& reg) {
	auto view = reg.view<GridElem, Renderable, UpdateRendPositionFlag>();
	for (auto id : view) {
		auto [grid, rend] = view.get(id);
		for (auto& sptr : rend) {
			sptr->Pos = GetSpritePos(grid.Pos);
		}
		reg.remove<UpdateRendPositionFlag>(id);
	}
}

void LoadLevel(const std::string& name, entt::registry& reg, Renderer& render, MapView& map) {
	LoadLevelData(reg, name);
	UpdateMap(reg, map);

	LoadTiling(reg);
	UpdateTiling(reg, map);
	
	UpdateRenderer(reg, render);
	UpdateRenderPositions(reg);
}
