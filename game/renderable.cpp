#include "renderable.h"

#include "batching.h"
#include "map.h"
/*
void ResetRenders(std::unordered_map<uint32_t, BatchedRender>& renders) {
	for (auto& el : renders) {
		el.second.Reset();
	}
}

void ClearRenders(std::unordered_map<uint32_t, BatchedRender>& renders) {
	for (auto& el : renders) {
		el.second.Clear();
	}
}
*/
/*
void RenderObstructSystem(entt::registry& db, BatchedRender& render) {
	auto walls = db.view<Wall, MultiRenderable>();
	for (auto id : walls) {
		Sprite sprite;
		sprite.Load(walls.get<MultiRenderable>(id).Images.front().DataId);
		sprite.Color = ColorRGBA{ 0, 0, 255, 255 };
		sprite.Pos = walls.get<Wall>(id).Pos * static_cast<int>(Resources().TileSize);
		sprite.Pos += glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
		render.Stage(sprite);
	}
	render.Draw();
}
*/
