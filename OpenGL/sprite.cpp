#include "sprite.h"
#include "resources.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

void Sprite::Load(std::string dataName) {
	Load(Resources().GetSpriteInfoId(dataName));
}

void Sprite::Load(uint32_t dataId) {
	DataId = dataId;
	Visible = true;
}

void Sprite::Reset() {
	DataId = 0;
	Pos = { 0, 0 };
	Visible = false;
}


void Sprite::PrepareForDraw() const {
	Resources().GetShader(Shaders::Sprite).Select();
	uint32_t textureId = Resources().GetSpriteInfo(DataId).TextureId;
	Resources().GetTexture(textureId).Select();

	glBindVertexArray(Resources().GetSpriteVAO());
}

void Sprite::ClearAfterDraw() const {
	glBindVertexArray(0);
}

void Sprite::Draw() const {
	if (!Visible)
		return;

	PrepareForDraw();
	FastDraw();
	ClearAfterDraw();
}

glm::vec4 Sprite::GetRect() const {
	const auto& info = Resources().GetSpriteInfo(DataId);
	return glm::vec4(Pos.x, Pos.y, info.Size.x, info.Size.y);
}

glm::vec4 Sprite::GetTextCoords() const {
	const auto& info = Resources().GetSpriteInfo(DataId);
	return glm::vec4(info.Position.x, info.Position.y, info.Size.x, info.Size.y);
}

glm::vec3 Sprite::GetTransform() const {
	const auto& info = Resources().GetSpriteInfo(DataId);
	return glm::vec3(info.Size * Scale, Rotation);
}

glm::vec2 Sprite::GetSize() const {
	const auto& info = Resources().GetSpriteInfo(DataId);
	return info.Size;
}

uint32_t Sprite::GetTextureId() const {
	return Resources().GetSpriteInfo(DataId).TextureId;
}

void Sprite::FastDraw() const {
	if (!Visible)
		return;

	Resources().GetShader(0).SetVec2("Position", Pos);
	Resources().GetShader(0).SetVec4("TextCoords", GetTextCoords());
	Resources().GetShader(0).SetVec4("Color", Color);
	Resources().GetShader(0).SetVec3("Transform", GetTransform());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}