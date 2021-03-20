#pragma once

#include <game/map.h>

#include <entt/entt.hpp>

void UpdateMoving(entt::registry& reg);
void UpdateMovingEditor(entt::registry& reg);

void MoveEntities(entt::registry& reg, MapView& map);

void UpdateFollowCamera(entt::registry& reg);
