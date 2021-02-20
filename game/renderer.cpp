#include "renderer.h"

#include "resources.h"

void Renderer::Load() {}

void Renderer::Reset() {
	renders.clear();
}

SpritePtr Renderer::Stage(size_t textureId) {
	BatchedRender& render = GetRenderForTexture(textureId);
	return render.Stage();
}

SpritePtr Renderer::Stage(const Sprite& sprite, size_t textureId) {
	BatchedRender& render = GetRenderForTexture(textureId);
	return render.Stage(sprite);
}

SpritePtr Renderer::Stage(const SpriteInfo& info) {
	return Stage(info.Value, info.TextureId);
}

SpritePtr Renderer::Stage(const std::string& name) {
	return Stage(Resources().GetSpriteInfo(name));
}

void Renderer::Draw() {
	for (auto& r : renders) {
		r.second->Draw();
	}
}

BatchedRender& Renderer::GetRenderForTexture(size_t textureId) {
	std::unique_ptr<BatchedRender>& ptr = renders[textureId];
	if (!ptr) {
		ptr = std::make_unique<BatchedRender>();
		ptr->Load(textureId);
	}
	return *ptr;
}
