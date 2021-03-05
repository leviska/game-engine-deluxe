#pragma once

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

struct CeilingObstruct {};
struct FrontWallObstruct {};
struct FloorObstruct {};

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

void UpdateWallSprite(glm::ivec2 pos, const MapView& map, entt::registry& reg, Renderer& render);
void UpdateWallSprite(entt::entity id, const MapView& map, entt::registry& reg, Renderer& render);

entt::entity CreateElement(glm::ivec2 pos, MapView& map, entt::registry& reg);
glm::vec2 GetSpritePos(glm::ivec2 pos);

void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const std::string& fileName);
void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const nlohmann::json& file);
void SaveMap(const MapView& map, const entt::registry& reg, const std::string& fileName);
void SaveMap(const MapView& map, const entt::registry& reg, nlohmann::json& result);
