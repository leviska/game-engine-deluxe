#include <graphics/renderer.h>

#include <graphics/camera.h>

#include <utility/casts.h>

#include <resources/shaders.h>
#include <resources/glbuffers.h>


Renderer::Renderer() : data(CompareSprites()) {}

void Renderer::Load() {
	glGenBuffers(1, &dataBuffer);
}

void Renderer::Reset() {
	data.Clear();
	textureIds.clear();
	textSearch.clear();
	glDeleteBuffers(1, &dataBuffer);
}

size_t Renderer::StageRaw(Sprite sprite, size_t textureId) {
	AddTextureIfNeeded(textureId);
	sprite.textureIndex = textSearch.at(textureId);
	return data.Push(sprite);
}

SpritePtr Renderer::Stage(const Sprite& sprite, size_t textureId) {
	return SpritePtr(StageRaw(sprite, textureId), this);
}

SpritePtr Renderer::Stage(size_t textureId) {
	return Stage(Sprite(), textureId);
}


SpritePtr Renderer::Stage(const SpriteInfo& info) {
	return Stage(static_cast<const Sprite&>(info), info.TextureId);
}

SpritePtr Renderer::Stage(const std::string& name) {
	return Stage(Graphics().Sprites[name]);
}

void Renderer::Unstage(size_t id) {
	data.Erase(id);
}

Sprite& Renderer::operator[](size_t id) {
	return data[id];
}

const Sprite& Renderer::operator[](size_t id) const {
	return data[id];
}

void Renderer::Draw(const Camera& camera) {
	const Shader& shader = Shaders().Shaders[to_ui32(ShadersId::Batch)];
	shader.Select();
	shader.SetMat4("Projection", camera.Matrix());

	if (data.Empty()) {
		return;
	}

	data.Sort();

	Buffers().BatchBuffer.Bind();
	for (size_t i = 0; i < textureIds.size(); i++) {
		Graphics().Textures[textureIds[i]].Select(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)0);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)8);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)12);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)20);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)28);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)36);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)52);
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)56);

	const auto& curData = data.Data();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite) * curData.size(), curData.data(), GL_STATIC_DRAW);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, curData.size());
}

void Renderer::AddTextureIfNeeded(size_t textId) {
	auto res = textSearch.insert({ textId, textureIds.size() });
	if (res.second) {
		textureIds.push_back(textId);
		assert(textureIds.size() < MaxTextures);
	}
}