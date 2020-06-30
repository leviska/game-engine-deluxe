#include "sprite.h"
#include "resources.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void Sprite::Load(uint32_t DataId) {
	dataId = DataId;
	visible = true;
}

void Sprite::Reset() {
	dataId = 0;
	pos = { 0, 0 };
	visible = false;
}


void Sprite::PrepareForDraw() {
	Resources().GetShader(Shaders::Sprite).Select();
	uint32_t textureId = Resources().GetSpriteInfo(dataId).TextureId;
	Resources().GetTexture(textureId).Select();

	glBindVertexArray(Resources().GetSpriteVAO());
}

void Sprite::ClearAfterDraw() {
	glBindVertexArray(0);
}

void Sprite::Draw() {
	if (!visible)
		return;

	PrepareForDraw();
	FastDraw();
	ClearAfterDraw();
}

glm::vec4 Sprite::GetRect() {
	const auto& info = Resources().GetSpriteInfo(dataId);
	return glm::vec4(pos.x, pos.y, info.Size.x, info.Size.y);
}

glm::mat4 Sprite::GetModel() {
	const auto& info = Resources().GetSpriteInfo(dataId);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, 0.0f));
	model = glm::scale(model, glm::vec3(info.Size, 1.0f));
	return model;
}

glm::vec4 Sprite::GetTextCoords() {
	const auto& info = Resources().GetSpriteInfo(dataId);
	return glm::vec4(info.Position.x, info.Position.y + info.Size.y, info.Size.x, -info.Size.y);
}

void Sprite::FastDraw() {
	if (!visible)
		return;

	Resources().GetShader(0).SetMat4("model", GetModel());
	Resources().GetShader(0).SetVec4("textCoords", GetTextCoords());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}