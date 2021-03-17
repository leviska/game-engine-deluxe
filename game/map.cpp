#include "map.h"

#include "serialization.h"

#include <imgui.h>

void GridElem::Load(const nlohmann::json& data) {
	data.at("Pos").get_to(Pos);
}

void GridElem::Save(nlohmann::json& data) const {
	data["Pos"] = Pos;
}


void GridElem::Edit() {
	ImGui::DragInt2("Position", &Pos[0]);
}
