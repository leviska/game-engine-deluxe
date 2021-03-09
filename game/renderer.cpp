#include "renderer.h"

#include "graphics.h"
#include "shaders.h"

#include "camera.h"

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
	return Stage(Graphics().Sprites[name]);
}

void Renderer::Draw(const Camera& camera) {
	const Shader& shader = Shaders().Shaders[to_ui32(ShadersId::Batch)];
	shader.Select();
	shader.SetMat4("Projection", camera.Matrix());
	for (auto& r : renders) {
		r.second.Draw();
	}
}

BatchedRender& Renderer::GetRenderForTexture(size_t textureId) {
	auto it = renders.emplace(textureId, BatchedRender());
	if (it.second) {
		it.first->second.Load(textureId);
	}
	return it.first->second;
}
