#pragma once

#include "sprite.h"

#include <unordered_map>
#include <vector>
#include <ostream>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

struct Wall {
	glm::ivec2 Pos;
};

class WallMap {
	using Map = std::unordered_map<int32_t, std::unordered_map<int32_t, entt::entity>>;

	Map map;
	entt::registry* db = nullptr;
public:
	void Load(entt::registry& _db);
	void Reset();

	entt::registry& DB() { return *db; }

	bool Has(glm::ivec2 pos) const;
	entt::entity Get(glm::ivec2 pos) const;

	void Update(entt::entity id, glm::ivec2 pos);

	void SetWall(glm::ivec2 pos, const std::vector<Sprite>& sprites);
};

void LoadMap(WallMap& map, const nlohmann::json& file);
void SaveMap(WallMap& map, nlohmann::json& result);