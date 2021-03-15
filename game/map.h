#pragma once

#include "map_components.h"
#include "hashes.h"
#include "renderer.h"

#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

class GameInst;

struct GridElem {
	glm::ivec2 Pos;

	inline static const std::string ComponentName{ "GridElem" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};

using MapView = std::unordered_map<glm::ivec2, std::vector<entt::entity>>;

template<typename Callback>
void CallForEvery(Callback& f, glm::ivec2 pos, const MapView& map) {
	auto it = map.find(pos);
	if (it == map.end()) {
		return;
	}
	for (auto id : it->second) {
		if (f(id)) {
			break;
		}
	}
}

// returns null if nothing found
template<typename Type>
entt::entity GetFirstOfType(glm::ivec2 pos, const MapView& map, const entt::registry& reg) {
	entt::entity res = entt::null;
	auto callback = [&](entt::entity id) {
		if (reg.has<Type>(id)) {
			res = id;
			return true;
		}
		return false;
	};
	CallForEvery(callback, pos, map);
	return res;
}
