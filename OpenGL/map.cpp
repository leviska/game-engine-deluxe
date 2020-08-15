#include "map.h"

#include "renderable.h"
#include "resources.h"

#include <fstream>

void WallMap::Load(entt::registry& _db) {
	db = &_db;
}

void WallMap::Reset() {
	map.clear();
}

bool WallMap::Has(glm::ivec2 pos) const {
	return map.find(pos) != map.end();
}

entt::entity WallMap::At(glm::ivec2 pos) const {
	if (Has(pos)) {
		return map.at(pos);
	}
	return entt::null;
}

entt::entity WallMap::Get(glm::ivec2 pos) {
	if (Has(pos)) {
		return map.at(pos);
	}
	else {
		return Create(pos);
	}
}

void WallMap::Update(glm::ivec2 pos, entt::entity id) {
	if (id == entt::null) {
		map.erase(pos);
	}
	else {
		map[pos] = id;
	}
}

entt::entity WallMap::Create(glm::ivec2 pos) {
	assert(!Has(pos));

	entt::entity id = db->create();
	db->emplace<Wall>(id, pos);
	Update(pos, id);
	return id;
}

void WallMap::Erase(glm::ivec2 pos) {
	assert(Has(pos));

	entt::entity id = At(pos);
	db->destroy(id);
	Update(pos, entt::null);
}

entt::entity WallMap::Set(glm::ivec2 pos, bool erase) {
	if (!erase) {
		return Create(pos);
	}
	else {
		Erase(pos);
		return entt::null;
	}
}

void LoadMap(WallMap& map, const char* fileName) {
	LoadMap(map, std::string{ fileName });
}

void LoadMap(WallMap& map, const std::string& name) {
	std::string fileName = std::string("assets/") + name + std::string(".json");
	std::ifstream file(fileName);
	if (!file.good()) {
		throw std::runtime_error("Cannot open file " + fileName);
	}
	nlohmann::json level;
	file >> level;
	LoadMap(map, level);
}

void LoadMap(WallMap& map, const nlohmann::json& levelInfo) {
	const nlohmann::json& walls = levelInfo["Walls"];
	for (const auto& elem : walls) {
		glm::ivec2 pos{ elem["Pos"]["x"].get<int32_t>(), elem["Pos"]["y"].get<int32_t>() };
		std::vector<Sprite> sprites;
		if (elem.find("Sprites") != elem.end()) {
			Sprite sprite;
			sprite.Pos = glm::vec2{ pos.x * Resources().TileSize, pos.y * Resources().TileSize } +glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
			for (const auto& spriteId : elem["Sprites"]) {
				sprite.Load(spriteId.get<std::string>());
				sprites.push_back(sprite);
			}
		}
		entt::entity id = map.Create(pos);
		map.DB().emplace<MultiRenderable>(id, sprites);
	}
}

void SaveMap(WallMap& map, const char* fileName) {
	SaveMap(map, std::string{ fileName });
}

void SaveMap(WallMap& map, const std::string& name) {
	std::string fileName = std::string("assets/") + name + std::string(".json");
	std::ofstream file(fileName);
	if (!file.good()) {
		throw std::runtime_error("Cannot open file " + fileName);
	}
	nlohmann::json level;
	SaveMap(map, level);
	file << level;
}

void SaveMap(WallMap& map, nlohmann::json& result) {
	result = nlohmann::json::object();
	result["Walls"] = nlohmann::json::array();
	nlohmann::json& walls = result["Walls"];
	auto view = map.DB().view<Wall, MultiRenderable>();
	for (auto id : view) {
		glm::ivec2 pos = view.get<Wall>(id).Pos;
		const std::vector<Sprite> sprites = view.get<MultiRenderable>(id).Images;
		nlohmann::json obj;
		obj["Pos"] = { { "x", pos.x }, { "y", pos.y } };
		obj["Sprites"] = nlohmann::json::array();
		for (const auto& i : sprites) {
			obj["Sprites"].push_back(Resources().GetSpriteInfo(i.DataId).Name);
		}
		walls.push_back(obj);
	}
}
