#pragma once

#include "utility.h"
#include "renderer.h"

#include <unordered_map>
#include <vector>
#include <ostream>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

class GameInst;

struct GridElem {
	glm::ivec2 Pos;
};

using MapView = std::unordered_map<glm::ivec2, std::vector<entt::entity>>;

void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const std::string& fileName);
void LoadMap(MapView& map, entt::registry& reg, Renderer& render, const nlohmann::json& file);
void SaveMap(const MapView& map, const entt::registry& reg, const std::string& fileName);
void SaveMap(const MapView& map, const entt::registry& reg, nlohmann::json& result);
