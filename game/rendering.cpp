#include "rendering.h"

#include "resources.h"

void Rendering::Load() {}

void Rendering::Reset() {
	renders.clear();
}

SpritePtr Rendering::Stage(size_t textureId) {
	BatchedRender& render = GetRenderForTexture(textureId);
	size_t id = render.Stage();
	return SpritePtr(id, &render);
}

SpritePtr Rendering::Stage(const Sprite& sprite, size_t textureId) {
	BatchedRender& render = GetRenderForTexture(textureId);
	size_t id = render.Stage(sprite);
	return SpritePtr(id, &render);
}

SpritePtr Rendering::Stage(const std::string& name) {
	const SpriteInfo& info = Resources().GetSpriteInfo(name);
	return Stage(info.Value, info.TextureId);
}

void Rendering::Unstage(const SpritePtr& ptr) {
	if (!ptr)
		return;
	ptr.render->Unstage(ptr.spriteId);
}

void Rendering::Draw() {
	for (auto& r : renders) {
		r.second->Draw();
	}
}

BatchedRender& Rendering::GetRenderForTexture(size_t textureId) {
	std::unique_ptr<BatchedRender>& ptr = renders[textureId];
	if (!ptr) {
		ptr = std::make_unique<BatchedRender>();
		ptr->Load(textureId);
	}
	return *ptr;
}
