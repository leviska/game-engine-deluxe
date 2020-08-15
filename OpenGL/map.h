#pragma once

#include "sprite.h"

#include "utility.h"

#include <unordered_map>
#include <vector>
#include <ostream>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

struct Wall {
	glm::ivec2 Pos;
};

class WallMap {
	using Map = std::unordered_map<glm::ivec2, entt::entity>;

	Map map;
	entt::registry* db = nullptr;

	void Update(glm::ivec2 pos, entt::entity id);
public:
	void Load(entt::registry& _db);
	void Reset();

	entt::registry& DB() { return *db; }

	bool Has(glm::ivec2 pos) const;
	entt::entity At(glm::ivec2 pos) const;
	entt::entity Get(glm::ivec2 pos);
	entt::entity Create(glm::ivec2 pos);
	void Erase(glm::ivec2 pos);
	entt::entity Set(glm::ivec2 pos, bool erase);
};

void LoadMap(WallMap& map, const char* fileName);
void LoadMap(WallMap& map, const std::string& fileName);
void LoadMap(WallMap& map, const nlohmann::json& file);
void SaveMap(WallMap& map, const char* fileName);
void SaveMap(WallMap& map, const std::string& fileName);
void SaveMap(WallMap& map, nlohmann::json& result);
