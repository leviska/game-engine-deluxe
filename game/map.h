#pragma once

#include "map_components.h"
#include "hashes.h"
#include "renderer.h"

#include <unordered_map>
#include <vector>
#include <bitset>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

class GameInst;

struct GridElem {
	glm::ivec2 Pos;
};

using MapView = std::unordered_map<glm::ivec2, std::vector<entt::entity>>;

enum class TilingId {
	R,
	BRFull,
	BRLeft,
	BRRight,
	BRCorner,
	B,
	BLFull,
	BLLeft,
	BLRight,
	BLCorner,
	L,
	TLFull,
	TLLeft,
	TLRight,
	TLCorner,
	T,
	TRFull,
	TRLeft,
	TRRight,
	TRCorner,
	Size
};

enum class NeighbourId {
	R,
	BR,
	B,
	BL,
	L,
	TL,
	T,
	TR,
	Size
};

using TilingBitset = std::bitset<static_cast<size_t>(TilingId::Size)>;
using NeighbourBitset = std::bitset<static_cast<size_t>(NeighbourId::Size)>;

TilingBitset GetTiling(NeighbourBitset neigh);

void UpdateSprite(glm::ivec2 pos, const MapView& map, entt::registry& reg, Renderer& render);
void UpdateSprite(entt::entity id, const MapView& map, entt::registry& reg, Renderer& render);

entt::entity CreateElement(glm::ivec2 pos, MapView& map, entt::registry& reg);
glm::vec2 GetSpritePos(glm::ivec2 pos);

void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const std::string& fileName);
void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const nlohmann::json& file);
void SaveMap(const MapView& map, const entt::registry& reg, const std::string& fileName);
void SaveMap(const MapView& map, const entt::registry& reg, nlohmann::json& result);

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
