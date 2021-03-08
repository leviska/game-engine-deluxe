#include "map.h"

#include "renderable.h"
#include "assertion.h"
#include "serialization.h"

#include "consts.h"
#include "graphics.h"
#include "paths.h"

#include <fstream>

glm::vec2 GetSpritePos(glm::ivec2 pos) {
	return glm::vec2(pos) * static_cast<float>(Consts().TileSize) + static_cast<float>(Consts().TileSize / 2);
}

TilingBitset GetTiling(NeighbourBitset neigh) {
	TilingBitset res;

	// middle tiles
	const std::array middleMap = { 
		std::pair{ NeighbourId::R, TilingId::R },
		std::pair{ NeighbourId::B, TilingId::B },
		std::pair{ NeighbourId::L, TilingId::L },
		std::pair{ NeighbourId::T, TilingId::T }
	};

	for (const auto& p : middleMap) {
		res[to_ui32(p.second)] = !neigh[to_ui32(p.first)];
	}

	// corner tiles
	const std::array cornerNeighMap = {
		std::array{ NeighbourId::R, NeighbourId::BR, NeighbourId::B },
		std::array{ NeighbourId::B, NeighbourId::BL, NeighbourId::L },
		std::array{ NeighbourId::L, NeighbourId::TL, NeighbourId::T },
		std::array{ NeighbourId::T, NeighbourId::TR, NeighbourId::R },
	};

	const std::array cornerTileMap = {
		std::array{ TilingId::BRFull, TilingId::BRLeft, TilingId::BRRight, TilingId::BRCorner },
		std::array{ TilingId::BLFull, TilingId::BLLeft, TilingId::BLRight, TilingId::BLCorner },
		std::array{ TilingId::TLFull, TilingId::TLLeft, TilingId::TLRight, TilingId::TLCorner },
		std::array{ TilingId::TRFull, TilingId::TRLeft, TilingId::TRRight, TilingId::TRCorner },
	};

	for (int i = 0; i < 4; i++) {
		int type = neigh[to_ui32(cornerNeighMap[i][0])] * 4 + neigh[to_ui32(cornerNeighMap[i][1])] + neigh[to_ui32(cornerNeighMap[i][2])] * 2;
		TilingId id;
		if (type <= 1) {
			id = cornerTileMap[i][0];
		}
		else if (type >= 2 && type <= 3) {
			id = cornerTileMap[i][1];
		}
		else if (type >= 4 && type <= 5) {
			id = cornerTileMap[i][2];
		}
		else if (type == 6) {
			id = cornerTileMap[i][3];
		}
		else {
			continue;
		}
		res[to_ui32(id)] = true;
	}

	return res;
}

SpritePtr UpdateSpriteWithName(Renderer& render, Renderable& rend, glm::ivec2 pos, const std::string& name) {
	SpritePtr sptr = render.Stage(Graphics().Sprites[name]);
	sptr->Pos = GetSpritePos(pos);
	rend.emplace_back(sptr);
	return sptr;
}

void UpdateCeiling(const MapView& map, entt::registry& reg, Renderer& render, Renderable& rend, glm::ivec2 pos) {
	NeighbourBitset neigh; // right, bottom right, bottom, ...
	const std::array shifts = { 
		glm::ivec2{ 1, 0 }, glm::ivec2{ 1, 1 }, glm::ivec2{ 0, 1 }, glm::ivec2{ -1, 1 }, 
		glm::ivec2{ -1, 0 }, glm::ivec2{ -1, -1 }, glm::ivec2{ 0, -1 }, glm::ivec2{ 1, -1 }
	};
	for (int i = 0; i < 8; i++) {
		glm::ivec2 cur = pos + shifts[i];
		neigh[i] = GetFirstOfType<TilingMap>(cur, map, reg) != entt::null;
	}

	const std::array spriteMap = {
		std::pair{ TilingId::R, std::string{"TilingR"} },
		std::pair{ TilingId::BRFull, std::string{"TilingBRFull"} },
		std::pair{ TilingId::BRLeft, std::string{"TilingBRLeft"} },
		std::pair{ TilingId::BRRight, std::string{"TilingBRRight"} },
		std::pair{ TilingId::BRCorner, std::string{"TilingBRCorner"} },
		std::pair{ TilingId::B, std::string{"TilingB"} },
		std::pair{ TilingId::BLFull, std::string{"TilingBLFull"} },
		std::pair{ TilingId::BLLeft, std::string{"TilingBLLeft"} },
		std::pair{ TilingId::BLRight, std::string{"TilingBLRight"} },
		std::pair{ TilingId::BLCorner, std::string{"TilingBLCorner"} },
		std::pair{ TilingId::L, std::string{"TilingL"} },
		std::pair{ TilingId::TLFull, std::string{"TilingTLFull"} },
		std::pair{ TilingId::TLLeft, std::string{"TilingTLLeft"} },
		std::pair{ TilingId::TLRight, std::string{"TilingTLRight"} },
		std::pair{ TilingId::TLCorner, std::string{"TilingTLCorner"} },
		std::pair{ TilingId::T, std::string{"TilingT"} },
		std::pair{ TilingId::TRFull, std::string{"TilingTRFull"} },
		std::pair{ TilingId::TRLeft, std::string{"TilingTRLeft"} },
		std::pair{ TilingId::TRRight, std::string{"TilingTRRight"} },
		std::pair{ TilingId::TRCorner, std::string{"TilingTRCorner"} },
	};

	TilingBitset tiles = GetTiling(neigh);
	for (const auto& p : spriteMap) {
		if (tiles[to_ui32(p.first)]) {
			UpdateSpriteWithName(render, rend, pos, p.second);
		}
	}
}

void UpdateFloor(Renderer& render, Renderable& rend, glm::ivec2 pos) {
	SpritePtr sptr = render.Stage(Graphics().Sprites["Square"]);
	sptr->Pos = GetSpritePos(pos);
	sptr->Scale = { 16.0f, 16.0f };
	sptr->Color = RGBA({ 31, 14, 28, 255 });
	rend.emplace_back(sptr);
}

void UpdateSprite(glm::ivec2 pos, const MapView& map, entt::registry& reg, Renderer& render) {
	auto callback = [&](entt::entity id) {
		UpdateSprite(id, map, reg, render);
		return false;
	};
	CallForEvery(callback, pos, map);
}

void UpdateSprite(entt::entity id, const MapView& map, entt::registry& reg, Renderer& render) {
	if (!reg.has<GridElem>(id) || !reg.has<Renderable>(id)) {
		return;
	}
	glm::ivec2 pos = reg.get<GridElem>(id).Pos;
	Renderable& rend = reg.get<Renderable>(id);
	rend.clear();

	if (reg.has<FrontWall>(id)) {
		UpdateSpriteWithName(render, rend, pos, "FrontWall");
	}
	else if (reg.has<TilingMap>(id)) {
		UpdateCeiling(map, reg, render, rend, pos);
	}
	else if (reg.has<Floor>(id)) {
		UpdateFloor(render, rend, pos);
	}
	else if (reg.has<Player>(id)) {
		SpritePtr sptr = UpdateSpriteWithName(render, rend, pos, "Player");
		sptr->Layer = 2;
	}
	else if (reg.has<Orc>(id)) {
		SpritePtr sptr = UpdateSpriteWithName(render, rend, pos, "Orc");
		sptr->Layer = 2;
	}
	else if (reg.has<Ladder>(id)) {
		SpritePtr sptr = UpdateSpriteWithName(render, rend, pos, "Ladder");
		sptr->Layer = 1;
	}
	else if (reg.has<Rock>(id)) {
		SpritePtr sptr = UpdateSpriteWithName(render, rend, pos, "Rock");
		sptr->Layer = 1;
	}
	else if (reg.has<Path>(id)) {
		SpritePtr sptr = UpdateSpriteWithName(render, rend, pos, "Path");
		sptr->Layer = 1;
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
				reg.emplace<TilingMap>(id);
			}
			else if (elem["Type"] == "FrontWall") {
				reg.emplace<FrontWall>(id);
			}
			else if (elem["Type"] == "Floor") {
				reg.emplace<Floor>(id);
			}
		}
	}
	for (const auto& vec : map) {
		for (auto id : vec.second) {
			UpdateSprite(id, map, reg, render);
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
			if (reg.has<TilingMap>(id)) {
				type = "Ceiling";
			}
			else if (reg.has<FrontWall>(id)) {
				type = "FrontWall";
			}
			else if (reg.has<Floor>(id)) {
				type = "Floor";
			}
			obj["Type"] = type;
			walls.push_back(obj);
		}
	}
}
