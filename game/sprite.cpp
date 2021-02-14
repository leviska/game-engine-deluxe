#include "sprite.h"
#include "resources.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite(const std::string& dataName) {
	Id = Resources().GetSpriteInfoId(dataName);
}

glm::vec4 Sprite::GetRect() const {
	const auto& info = Resources().GetSpriteInfo(Id);
	return glm::vec4(Pos, info.Size);
}

glm::vec4 Sprite::GetTextCoords() const {
	const auto& info = Resources().GetSpriteInfo(Id);
	return glm::vec4(info.Position, info.Size);
}

glm::vec3 Sprite::GetTransform() const {
	const auto& info = Resources().GetSpriteInfo(Id);
	return glm::vec3(info.Size * Scale, Rotation);
}

glm::vec2 Sprite::GetSize() const {
	const auto& info = Resources().GetSpriteInfo(Id);
	return info.Size;
}

uint32_t Sprite::GetTextureId() const {
	return Resources().GetSpriteInfo(Id).TextureId;
}

void DrawSprite(const Sprite& sprite) {
	if (!sprite.Visible)
		return;
	DrawSprite(sprite.Pos, sprite.Id, sprite.Color, sprite.Scale, sprite.Rotation);
}

void DrawSprite(
	glm::vec2 pos,
	uint32_t dataId,
	ColorType color,
	float scale,
	float rotation)
{
	const auto& info = Resources().GetSpriteInfo(dataId);
	DrawSprite(pos, Resources().GetTexture(info.TextureId), info.Size, info.Position, color, scale, rotation);
}

void DrawSprite(
	glm::vec2 pos,
	const Texture& texture,
	glm::vec2 size,
	glm::vec2 textPos,
	ColorType color,
	float scale,
	float rotation,
	Shaders shaderId)
{
	Resources().GetShader(shaderId).Select();
	texture.Select();
	glBindVertexArray(Resources().GetSpriteVAO());

	Resources().GetShader(Shaders::Sprite).SetVec2("Position", pos);
	Resources().GetShader(Shaders::Sprite).SetVec4("TextCoords", glm::vec4(textPos, size));
	Resources().GetShader(Shaders::Sprite).SetVec4("Color", RGBA(color));
	Resources().GetShader(Shaders::Sprite).SetVec3("Transform", glm::vec3(size * scale, rotation));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
