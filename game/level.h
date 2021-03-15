#pragma once

#include "renderer.h"
#include "map.h"

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#define LevelComponents \
	Obstruct, \
	Floor, \
	Player, \
	Orc, \
	Ladder, \
	Bridge, \
	TilableSpriteData, \
	SimpleSpriteData

void LoadLevelData(entt::registry& reg, const std::string& name);
void SaveLevelData(const entt::registry& reg, const std::string& name);

glm::vec2 GetSpritePos(glm::ivec2 pos);

void UpdateRenderer(entt::registry& reg, entt::entity id, Renderer& render);
void UpdateMap(entt::registry& reg, entt::entity id, MapView& map);

void LoadRenderer(entt::registry& reg, Renderer& render);
void LoadMap(entt::registry& reg, MapView& map);
void UpdateGridPositions(entt::registry& reg);

void LoadLevel(const std::string& name, entt::registry& reg, Renderer& render, MapView& map);
