#include "renderable.h"

#include "graphics.h"

#include <imgui.h>
#include <imgui_stdlib.h>


void SimpleSpriteData::Load(const nlohmann::json& data) {
	data.at("Name").get_to(Name);
}

void SimpleSpriteData::Save(nlohmann::json& data) const {
	data["Name"] = Name;
}


bool DrawTileButton(const SpriteInfo& info) {
	ImTextureID textId = reinterpret_cast<void*>(static_cast<intptr_t>(Graphics().Textures["EditorButtons2D"].GetId()));
	glm::vec2 textSize(Graphics().Textures["EditorButtons2D"].GetSize());
	glm::vec2 sizeV = info.TextSize * 4.0f;
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


void SimpleSpriteData::Edit() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldSpacing = style.ItemSpacing;
	style.ItemSpacing = { 0, 0 };

	const NamedVector<SpriteInfo>& info = Graphics().EditorButtons;
	uint32_t currentLineDrawed = 0;
	bool firstDraw = true;
	for (size_t i = 0; i < info.Size(); i++) {
		if (info[i].Name == "Square") {
			continue;
		}
		ImGui::PushID(static_cast<int>(i));
		if (currentLineDrawed >= 3) {
			currentLineDrawed = 0;
		}
		else if (!firstDraw) {
			currentLineDrawed++;
			ImGui::SameLine();
		}
		firstDraw = false;

		if (DrawTileButton(info[i])) {
			Name = info[i].Name;
		}

		ImGui::PopID();
	}

	style.ItemSpacing = oldSpacing;
}


void TilableSpriteData::Load(const nlohmann::json& data) {
	std::string tmpData;
	data.at("Data").get_to(tmpData);
	Data = TilingBitset(tmpData);

	data.at("Prefix").get_to(Prefix);
}

void TilableSpriteData::Save(nlohmann::json& data) const {
	data["Data"] = Data.to_string();
	data["Prefix"] = Prefix;
}

void TilableSpriteData::Edit() {
	ImGui::InputText("Tiling prefix", &Prefix);
}
