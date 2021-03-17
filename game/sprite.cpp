#include "sprite.h"

#include "serialization.h"

#include "color.h"
#include "graphics.h"

#include <imgui.h>
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


void from_json(const nlohmann::json& j, Sprite& info) {
	j.at("Pos").get_to(info.Pos);
	j.at("Layer ").get_to(info.Layer);
	j.at("TextPos").get_to(info.TextPos);
	j.at("TextSize").get_to(info.TextSize);
	j.at("Scale").get_to(info.Scale);
	j.at("Color").get_to(info.Color);
	j.at("Rotation ").get_to(info.Rotation);
}

void to_json(nlohmann::json& j, const Sprite& info) {
	j["Pos"] = info.Pos;
	j["Layer "] = info.Layer;
	j["TextPos"] = info.TextPos;
	j["TextSize"] = info.TextSize;
	j["Scale"] = info.Scale;
	j["Color"] = info.Color;
	j["Rotation "] = info.Rotation;
}


void EditSprite(Sprite& sprite) {
	ImGui::InputFloat("Layer", &sprite.Layer);
	ImGui::InputFloat("Rotation", &sprite.Rotation);
	ImGui::InputFloat2("Scale", &sprite.Scale[0]);
	ImGui::ColorPicker4("Color", &sprite.Color[0]);
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