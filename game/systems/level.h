#pragma once

#include <graphics/renderer.h>
#include <game/map.h>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

void LoadLevelData(entt::registry& reg, const std::string& name);
void SaveLevelData(const entt::registry& reg, const std::string& name);

void UpdateRenderer(entt::registry& reg, Renderer& render);
void UpdateMap(entt::registry& reg, MapView& map);
bool MapValid(const entt::registry& reg, const MapView& map);
void UpdateRenderPositions(entt::registry& reg);

void LoadLevel(const std::string& name, entt::registry& reg, Renderer& render, MapView& map);
