#pragma once

#include <utility/hashes.h>

#include <graphics/renderer.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

using MapView = std::unordered_map<glm::ivec2, std::unordered_set<entt::entity>>;

struct GridElem {
	glm::ivec2 Pos;

	void ClearGridElem(entt::entity id, MapView& map);
	void UpdateGridElem(entt::entity id, MapView& map);


	inline static const std::string ComponentName{ "GridElem" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};

template<typename Callback>
void ForEachMapEntity(Callback& f, glm::ivec2 pos, const MapView& map) {
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
entt::entity GetFirstTypeInMap(glm::ivec2 pos, const MapView& map, const entt::registry& reg) {
	entt::entity res = entt::null;
	auto callback = [&](entt::entity id) {
		if (reg.has<Type>(id)) {
			res = id;
			return true;
		}
		return false;
	};
	ForEachMapEntity(callback, pos, map);
	return res;
}
