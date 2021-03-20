#include <components/movable.h>

#include <engine/serialization.h>

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

void GridElem::ClearGridElem(entt::entity id, MapView& map) {
	auto& set = map.at(Pos);
	set.erase(id);
	if (set.empty()) {
		map.erase(Pos);
	}
}

void GridElem::UpdateGridElem(entt::entity id, MapView& map) {
	map[Pos].insert(id);
}
