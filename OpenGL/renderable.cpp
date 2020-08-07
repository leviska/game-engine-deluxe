#include "renderable.h"

#include "batching.h"

void RenderSystem(entt::registry& db, BatchedRender& renderer) {
	auto rendView = db.view<Renderable>();
	for (auto id : rendView) {
		const Sprite& sprite = rendView.get<Renderable>(id).Image;
		renderer.Stage(sprite);
	}
	auto maskView = db.view<MaskRenderable>();
	for (auto id : maskView) {
		const std::vector<Sprite>& sprite = maskView.get<MaskRenderable>(id).Images;
		renderer.Stage(sprite);
	}
}