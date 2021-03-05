#include "map.h"

#include "renderable.h"
#include "assertion.h"
#include "serialization.h"

#include "consts.h"
#include "graphics.h"
#include "paths.h"

#include <fstream>
#include <bitset>

glm::vec2 GetSpritePos(glm::ivec2 pos) {
	return glm::vec2(pos) * static_cast<float>(Consts().TileSize) + static_cast<float>(Consts().TileSize / 2);
}

void UpdateWallSprite(glm::ivec2 pos, const MapView& map, entt::registry& reg, Renderer& render) {
	if (map.find(pos) == map.end() || !map.at(pos).empty() == 0) {
		return;
	}
	UpdateWallSprite(map.at(pos)[0], map, reg, render);
}

void UpdateFrontWall(Renderer& render, Renderable& rend, glm::ivec2 pos) {
	SpritePtr sptr = render.Stage(Graphics().Sprites[20]);
	sptr->Pos = GetSpritePos(pos);
	rend.emplace_back(sptr);
}

void UpdateCeiling(const MapView& map, entt::registry& reg, Renderer& render, Renderable& rend, glm::ivec2 pos) {
	std::bitset<8> neigh; // right, bottom right, bottom, ...
	const std::array<glm::ivec2, 8> shifts = { glm::ivec2{ 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 } };
	for (int i = 0; i < 8; i++) {
		glm::ivec2 cur = pos + shifts[i];
		auto it = map.find(cur);
		neigh[i] = it != map.end() && !it->second.empty() && reg.has<CeilingObstruct>(it->second[0]);
	}

	// middle tiles
	const std::unordered_map<int, int> mapping = { { 0, 15 }, { 2, 0 }, { 4, 5 }, { 6, 10 } };
	for (int i = 0; i < 8; i += 2) {
		if (neigh[i])
			continue;
		SpritePtr sptr = render.Stage(Graphics().Sprites[mapping.at(i)]);
		sptr->Pos = GetSpritePos(pos);
		rend.emplace_back(sptr);
	}

	const std::unordered_map<int, int> cormap = { { 1, 16 }, { 3, 1 }, { 5, 6 }, { 7, 11 } };
	for (int i = 1; i < 8; i += 2) {
		int type = neigh[i] + neigh[(i + 1) % 8] * 2 + neigh[(i + 7) % 8] * 4;
		int spr = 0;
		if (type <= 1) {
			spr = 0;
		}
		else if (type >= 2 && type <= 3) {
			spr = 2;
		}
		else if (type >= 4 && type <= 5) {
			spr = 3;
		}
		else if (type == 6) {
			spr = 1;
		}
		else {
			continue;
		}
		SpritePtr sptr = render.Stage(Graphics().Sprites[cormap.at(i) + spr]);
		sptr->Pos = GetSpritePos(pos);
		rend.emplace_back(sptr);
	}
}

void UpdateFloor(Renderer& render, Renderable& rend, glm::ivec2 pos) {
	SpritePtr sptr = render.Stage(Graphics().Sprites["Square"]);
	sptr->Pos = GetSpritePos(pos);
	sptr->Scale = { 16.0f, 16.0f };
	sptr->Color = RGBA({ 31, 14, 28, 255 });
	rend.emplace_back(sptr);
}

void UpdateWallSprite(entt::entity id, const MapView& map, entt::registry& reg, Renderer& render) {
	if (!reg.has<GridElem>(id) || !reg.has<Renderable>(id)) {
		return;
	}
	glm::ivec2 pos = reg.get<GridElem>(id).Pos;
	Renderable& rend = reg.get<Renderable>(id);
	rend.clear();

	if (reg.has<FrontWallObstruct>(id)) {
		UpdateFrontWall(render, rend, pos);
	}
	else if (reg.has<CeilingObstruct>(id)) {
		UpdateCeiling(map, reg, render, rend, pos);
	}
	else if (reg.has<FloorObstruct>(id)) {
		UpdateFloor(render, rend, pos);
	}
}

entt::entity CreateElement(glm::ivec2 pos, MapView& map, entt::registry& reg) {
	entt::entity id = reg.create();
	map[pos].push_back(id);
	reg.emplace<GridElem>(id, pos);
	reg.emplace<Renderable>(id);
	return id;
}

void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const std::string& name) {
	std::string fileName = Paths::Levels + name + std::string(".json");
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
		glm::ivec2 pos = elem["Pos"].get<glm::ivec2>();
		entt::entity id = CreateElement(pos, map, reg);
		if (elem.find("Type") != elem.end()) {
			if (elem["Type"] == "Ceiling") {
				reg.emplace<CeilingObstruct>(id);
			}
			else if (elem["Type"] == "FrontWall") {
				reg.emplace<FrontWallObstruct>(id);
			}
			else if (elem["Type"] == "Floor") {
				reg.emplace<FloorObstruct>(id);
			}
		}
	}
	for (const auto& vec : map) {
		for (auto id : vec.second) {
			UpdateWallSprite(id, map, reg, render);
		}
	}
}

void SaveMap(const MapView& map, const entt::registry& reg, const std::string& name) {
	std::string fileName = Paths::Assets + name + std::string(".json");
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
			obj["Pos"] = p.first;
			std::string type;
			if (reg.has<CeilingObstruct>(id)) {
				type = "Ceiling";
			}
			else if (reg.has<FrontWallObstruct>(id)) {
				type = "FrontWall";
			}
			else if (reg.has<FloorObstruct>(id)) {
				type = "Floor";
			}
			obj["Type"] = type;
			walls.push_back(obj);
		}
	}
}
