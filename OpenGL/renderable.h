#pragma once

#include "sprite.h"
#include "animated_sprite.h"
#include "batching.h"

#include <vector>
#include <unordered_map>

#include <entt/entt.hpp>

struct Renderable {
	Sprite Image;
};

struct AnimatedRenderable {
	AnimatedSprite Image;
};

struct MultiRenderable {
	std::vector<Sprite> Images;
};

struct MultiAnimatedRenderable {
	std::vector<AnimatedSprite> Images;
};

void ResetRenders(std::unordered_map<uint32_t, BatchedRender>& renders);
void ClearRenders(std::unordered_map<uint32_t, BatchedRender>& renders);

void RenderSystem(entt::registry& db, std::unordered_map<uint32_t, BatchedRender>& renders);
void RenderObstructSystem(entt::registry& db, BatchedRender& render);
