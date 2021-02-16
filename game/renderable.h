#pragma once

#include "renderable.h"
#include "sprite_ptr.h"
#include "animated_sprite.h"

#include <vector>
#include <unordered_map>

#include <entt/entt.hpp>

using Renderable = SpritePtr;
using MultiRenderable = std::vector<SpritePtr>;
/*
void ResetRenders(Renderable& render);
void ClearRenders(Renderable& render);

void RenderSystem(entt::registry& db, std::unordered_map<uint32_t, BatchedRender>& renders);
*/
//void RenderObstructSystem(entt::registry& db, BatchedRender& render);
