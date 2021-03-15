#include "sprite.h"

#include "color.h"
#include "graphics.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite(const SpriteInfo& sprite) {
	Copy(static_cast<const Sprite&>(sprite));
}

Sprite& Sprite::operator=(const SpriteInfo& sprite) {
	Copy(static_cast<const Sprite&>(sprite));
	return *this;
}


void Sprite::Copy(const Sprite& sprite) {
	Pos = sprite.Pos;
	Layer = sprite.Layer;
	TextPos = sprite.TextPos;
	TextSize = sprite.TextSize;
	Scale = sprite.Scale;
	Color = sprite.Color;
	Rotation = sprite.Rotation;
}

void HideSprite(Sprite& sprite) {
	sprite.Color = Colors::Invisible;
}

void ShowSprite(Sprite& sprite) {
	sprite.Color = Colors::White;
}

void SwitchVisibility(Sprite& sprite) {
	sprite.Color = Colors::White - sprite.Color;
}


/*
void DrawSprite(const Sprite& sprite) {
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
*/