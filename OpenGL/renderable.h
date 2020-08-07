#pragma once

#include "sprite.h"
#include "batching.h"

#include <vector>

#include <entt/entt.hpp>

struct Renderable {
	Sprite Image;
};

struct MaskRenderable {
	std::vector<Sprite> Images;
};

void RenderSystem(entt::registry& db, BatchedRender& renderer);