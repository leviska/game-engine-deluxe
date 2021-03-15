#include "map.h"

#include <imgui.h>

void GridElem::Load(const nlohmann::json& data) {}

void GridElem::Save(nlohmann::json& data) const {}

void GridElem::Edit() {
	ImGui::SliderInt2("Position", &Pos[0], 0, 400);
}