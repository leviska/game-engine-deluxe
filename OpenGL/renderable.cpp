#include "renderable.h"

#include "batching.h"
#include "map.h"

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

template <typename SpriteType>
void RenderSprite(const SpriteType& sprite, std::unordered_map<uint32_t, BatchedRender>& renders) {
	auto it = renders.find(sprite.GetTextureId());
	if (it != renders.end()) {
		it->second.Stage(sprite);
	}
	else {
		sprite.Draw();
	}
}

template <typename RenderableType>
void RenderRenderable(entt::registry& db, std::unordered_map<uint32_t, BatchedRender>& renders) {
	auto rendView = db.view<RenderableType>();
	for (auto id : rendView) {
		const auto& sprite = rendView.get<RenderableType>(id).Image;
		RenderSprite(sprite, renders);
	}
}

template <typename MultiRenderableType>
void RenderMultiRenderable(entt::registry& db, std::unordered_map<uint32_t, BatchedRender>& renders) {
	auto rendView = db.view<MultiRenderableType>();
	for (auto id : rendView) {
		const auto& images = rendView.get<MultiRenderableType>(id).Images;
		for (const auto& sprite : images) {
			RenderSprite(sprite, renders);
		}
	}
}

void RenderSystem(entt::registry& db, std::unordered_map<uint32_t, BatchedRender>& renders) {
	RenderRenderable<Renderable>(db, renders);
	RenderRenderable<AnimatedRenderable>(db, renders);
	
	RenderMultiRenderable<MultiRenderable>(db, renders);
	RenderMultiRenderable<MultiAnimatedRenderable>(db, renders);

	for (auto& el : renders) {
		el.second.Draw();
	}
}

void RenderObstructSystem(entt::registry& db, BatchedRender& render) {
	auto walls = db.view<Wall, MultiRenderable>();
	for (auto id : walls) {
		Sprite sprite;
		sprite.Load(walls.get<MultiRenderable>(id).Images.front().DataId);
		sprite.Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		sprite.Pos = walls.get<Wall>(id).Pos * static_cast<int>(Resources().TileSize);
		sprite.Pos += glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
		render.Stage(sprite);
	}
	render.Draw();
}
