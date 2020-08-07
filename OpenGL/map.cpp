#include "map.h"

#include "renderable.h"
#include "resources.h"


void WallMap::Load(entt::registry& _db) {
	db = &_db;
}

void WallMap::Reset() {
	map.clear();
}

bool WallMap::Has(glm::ivec2 pos) const {
	return map.find(pos.x) != map.end() && map.at(pos.x).find(pos.y) != map.at(pos.x).end();
}

entt::entity WallMap::Get(glm::ivec2 pos) const {
	if (Has(pos)) {
		return map.at(pos.x).at(pos.y);
	}
	return entt::null;
}

void WallMap::Update(entt::entity id, glm::ivec2 pos) {
	if (id == entt::null) {
		map[pos.x].erase(pos.y);
		if (map[pos.x].empty()) {
			map.erase(pos.x);
		}
	}
	else {
		map[pos.x][pos.y] = id;
	}
}

void WallMap::SetWall(glm::ivec2 pos, const std::vector<Sprite>& sprites) {
	if (Has(pos)) {
		if (sprites.empty()) {
			// TODO:
			Update(entt::null, pos);
		}
		else {
			db->get<MaskRenderable>(map[pos.x][pos.y]).Images = sprites;
		}
	}
	else {
		entt::entity id = db->create();
		db->emplace<Wall>(id, pos);
		db->emplace<MaskRenderable>(id, sprites);
		Update(id, pos);
	}
}

void LoadMap(WallMap& map, const nlohmann::json& levelInfo) {
	const nlohmann::json& walls = levelInfo["Walls"];
	for (const auto& elem : walls) {
		glm::ivec2 pos{ elem["Pos"]["x"].get<int32_t>(), elem["Pos"]["y"].get<int32_t>() };
		std::vector<Sprite> sprites;
		if (elem.find("Sprites") != elem.end()) {
			Sprite sprite;
			sprite.Scale = Resources().Scale;
			sprite.Pos = glm::vec2{ pos.x * Resources().TileSize, pos.y * Resources().TileSize } +glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
			for (const auto& spriteId : elem["Sprites"]) {
				sprite.Load(spriteId.get<std::string>());
				sprites.push_back(sprite);
			}
		}
		map.SetWall(pos, sprites);
	}
}

void SaveMap(WallMap& map, nlohmann::json& result) {
	result = nlohmann::json::object();
	result["Walls"] = nlohmann::json::array();
	nlohmann::json& walls = result["Walls"];
	auto view = map.DB().view<Wall, MaskRenderable>();
	for (auto id : view) {
		glm::ivec2 pos = view.get<Wall>(id).Pos;
		const std::vector<Sprite> sprites = view.get<MaskRenderable>(id).Images;
		nlohmann::json obj;
		obj["Pos"] = { { "x", pos.x }, { "y", pos.y } };
		obj["Sprites"] = nlohmann::json::array();
		for (const auto& i : sprites) {
			obj["Sprites"].push_back(Resources().GetSpriteInfo(i.DataId).Name);
		}
		walls.push_back(obj);
	}
}
