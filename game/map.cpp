#include "map.h"

#include "renderable.h"
#include "resources.h"
#include "assertion.h"

#include <fstream>


void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const std::string& fileName) {
	std::string fileName = std::string("assets/") + fileName + std::string(".json");
	std::ifstream file(fileName);
	if (!file.good()) {
		THROWERROR("Cannot open file " + fileName);
	}
	nlohmann::json level;
	file >> level;
	LoadMap(map, reg, render, level);
}

void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const nlohmann::json& file) {
	const nlohmann::json& walls = file["Walls"];
	for (const auto& elem : walls) {
		glm::ivec2 pos{ elem["Pos"]["x"].get<int32_t>(), elem["Pos"]["y"].get<int32_t>() };
		entt::entity id = reg.create();
		map[pos].push_back(id);
		reg.emplace<GridElem>(id, pos);
		if (elem.find("Sprites") != elem.end()) {
			Renderable& rend = reg.emplace<Renderable>(id);
			for (const auto& spriteId : elem["Sprites"]) {
				SpritePtr sptr = render.Stage(spriteId.get<std::string>());
				sptr->Pos = glm::vec2{ pos.x * Resources().TileSize, pos.y * Resources().TileSize } +glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
				rend.emplace_back(sptr);
			}
		}
	}
}

void SaveMap(const MapView& map, const entt::registry& reg, const std::string& fileName) {
	std::string fileName = std::string("assets/") + fileName + std::string(".json");
	std::ofstream file(fileName);
	if (!file.good()) {
		THROWERROR("Cannot open file " + fileName);
	}
	nlohmann::json level;
	SaveMap(map, reg, level);
	file << level;
}

void SaveMap(const MapView& map, const entt::registry& reg, nlohmann::json& result) {
	result = nlohmann::json::object();
	result["Walls"] = nlohmann::json::array();
	nlohmann::json& walls = result["Walls"];
	for (const auto& p : map) {
		for (auto id : p.second) {
			nlohmann::json obj;
			obj["Pos"] = { { "x", p.first.x }, { "y", p.first.y } };
			obj["Sprites"] = nlohmann::json::array();
			if (reg.has<Renderable>(id)) {
				const Renderable& rend = reg.get<Renderable>(id);
				for (const auto& sptr : rend) {
					//obj["Sprites"].push_back(Resources().GetSpriteInfo(i.Id).Name);
				}
			}
			
			walls.push_back(obj);
		}
	}
}
