#include <components/tilable.h>

#include <resources/graphics.h>

#include <imgui.h>

void TilableData::Load(const nlohmann::json& data) {
	data.at("Name").get_to(Name);
}

void TilableData::Save(nlohmann::json& data) const {
	data["Name"] = Name;
}

void TilableData::Edit() {
	const std::vector<std::string> names = Graphics().TilingNames;
	for (const auto& name : names) {
		if (ImGui::RadioButton(name.c_str(), Name == name)) {
			Name = name;
		}
	}
}
