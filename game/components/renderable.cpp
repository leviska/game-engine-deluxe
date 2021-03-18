#include <components/renderable.h>

#include <resources/graphics.h>
#include <resources/editor_info.h>

#include <imgui.h>
#include <imgui_stdlib.h>


void SimpleSpriteData::Load(const nlohmann::json& data) {
	data.at("Name").get_to(Name);
}

void SimpleSpriteData::Save(nlohmann::json& data) const {
	data["Name"] = Name;
}


bool DrawTileButton(const SpriteInfo& info) {
	const Texture& text = Graphics().Textures2D[info.TextureId];
	ImTextureID textId = reinterpret_cast<void*>(static_cast<intptr_t>(text.GetId()));
	glm::vec2 textSize(text.GetSize());
	glm::vec2 sizeV = info.TextSize * info.Scale * 4.0f;
	glm::vec2 uv0V = info.TextPos;
	glm::vec2 uv1V = info.TextPos + info.TextSize;
	uv0V /= textSize;
	uv1V /= textSize;
	ImVec2 size{ sizeV.x, sizeV.y };
	ImVec2 uv0{ uv0V.x, uv0V.y };
	ImVec2 uv1{ uv1V.x, uv1V.y };
	ImVec4 bg_col = ImVec4(31.0f / 255.0f, 14.0f / 255.0f, 28.0f / 255.0f, 1.0f);
	return ImGui::ImageButton(textId, size, uv0, uv1, 1, bg_col);
}

int32_t ShowSpriteSelector() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldSpacing = style.ItemSpacing;
	style.ItemSpacing = { 0, 0 };

	const NamedVector<SpriteInfo>& info = Graphics().Sprites;
	const std::vector<std::string>& names = EditorInfo().SpriteNames;
	uint32_t currentLineDrawed = 0;
	bool firstDraw = true;

	int32_t res = -1;
	for (size_t i = 0; i < names.size(); i++) {
		ImGui::PushID(static_cast<int>(i));

		if (currentLineDrawed >= 3) {
			currentLineDrawed = 0;
		}
		else if (!firstDraw) {
			currentLineDrawed++;
			ImGui::SameLine();
		}
		firstDraw = false;

		if (DrawTileButton(info[names[i]])) {
			res = i;
		}

		ImGui::PopID();
	}

	style.ItemSpacing = oldSpacing;

	return res;
}


void SimpleSpriteData::Edit() {
	const std::vector<std::string>& names = EditorInfo().SpriteNames;
	int32_t id = ShowSpriteSelector();
	if (id >= 0) {
		Name = names[id];
	}
}


ComplexSpriteData::ComplexSpriteData() {
	Info = Graphics().Sprites["Square"];
}

void ComplexSpriteData::Load(const nlohmann::json& data) {
	data.at("Info").get_to(Info);
}

void ComplexSpriteData::Save(nlohmann::json& data) const {
	data["Info"] = Info;
}

void ComplexSpriteData::Edit() {
	const std::vector<std::string>& names = EditorInfo().SpriteNames;
	const NamedVector<SpriteInfo>& info = Graphics().Sprites;
	int32_t id = ShowSpriteSelector();
	if (id >= 0) {
		Info = info[names[id]];
	}
	EditSprite(Info);
}


void TilableSpriteData::Load(const nlohmann::json& data) {
	data.at("Id").get_to(Id);
}

void TilableSpriteData::Save(nlohmann::json& data) const {
	data["Id"] = Id;
}

void TilableSpriteData::Edit() {

}
