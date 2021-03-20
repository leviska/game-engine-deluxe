#include <game/editor.h>

#include <engine/input.h>

#include <utility/casts.h>

#include <systems/level.h>
#include <systems/moving.h>

#include <components/serialize.h>
#include <components/renderable.h>
#include <components/movable.h>
#include <components/utility.h>
#include <components/list.h>

#include <resources/consts.h>
#include <resources/graphics.h>
#include <resources/paths.h>
#include <resources/shaders.h>

#include <fstream>
#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <sdl2/SDL.h>


void EditorScene::Load(const std::string& name) {
	levelName = name;
	presetsName = "entity_presets.json";
	entityPresets = nlohmann::json::object();

	std::ifstream presetsFile(Paths::Editor + presetsName);
	if (presetsFile.good()) {
		presetsFile >> entityPresets;
	}

	LoadMap();

	camera = Camera{ {0, 0}, Consts().CanvasSize / 2u };

	frameBuffer.Load(Consts().CanvasSize);

	try {
		LoadLevel(levelName, reg, render, map);
	}
	catch (std::runtime_error exp) {
		// can't find file, ignore
	}

	bb.Color = ColorRGBA{ 255, 255, 255, 255 };
	curbb.Thickness = 2;
}

void EditorScene::Reset() {
	frameBuffer.Reset();
	levelName.clear();

	ResetMap();
}

void EditorScene::LoadMap() {
	render.Load();
}

void EditorScene::ResetMap() {
	map.clear();
	reg.clear();
	render.Reset();

	currentId = entt::null;
}

const std::string& EditorScene::CurrentLevel() const {
	return levelName;
}

void EditorScene::Update() {
	camera.UpdateFreeCamera();

	UpdateSystems();
	UpdateMouseEditor();
	UpdateBB();
}

void EditorScene::UpdateSystems() {
	UpdateRenderer(reg, render);
	assert(MapValid(reg, map));
	LoadTiling(reg);
	UpdateTiling(reg, map);
	if (currentId != entt::null) {
		reg.emplace<InputMovableEditorFlag>(currentId);
		UpdateMovingEditor(reg);
		MoveEntities(reg, map);
		reg.remove<InputMovableEditorFlag>(currentId);
	}
	UpdateRenderPositions(reg);
}

struct EditorTag {};

void EditorScene::UpdateMouseEditor() {
	glm::ivec2 mapPos = GlobalToGridPos(Input().MousePos(), camera);

	if (Input().KeyPressed(Mouse::Left)) {
		if (Input().KeyDown(Keyboard::LSHIFT)) {
			currentId = reg.create();
			
			reg.emplace<EditorTag>(currentId);
			if (curPreset.is_null()) {
				reg.emplace<GridElem>(currentId, mapPos).UpdateGridElem(currentId, map);
				reg.emplace<SimpleSpriteData>(currentId);
			}
			else {
				LoadEntity(LevelCompList(), currentId, curPreset["Components"], reg);

				GridElem* grid = reg.try_get<GridElem>(currentId);
				if (grid) {
					grid->Pos = mapPos;
					grid->UpdateGridElem(currentId, map);
				}
			}
		}
		else {
			auto it = map.find(mapPos);
			if (it != map.end() && !it->second.empty()) {
				const auto& values = it->second;
				auto vit = values.find(currentId);
				if (vit != values.end()) {
					vit++;
				}
				if (vit == values.end()) {
					vit = values.begin();
				}
				currentId = *vit;
			}
		}
	}
	if (Input().KeyPressed(Mouse::Right)) {
		auto it = map.find(mapPos);
		if (it != map.end() && !it->second.empty()) {
			auto& values = it->second;
			auto vit = values.find(currentId);
			if (vit == values.end()) {
				vit = values.begin();
			}
			reg.destroy(*vit);
			values.erase(vit);
			currentId = entt::null;
		}
	}
}

void EditorScene::UpdateBB() {
	glm::ivec2 mapPos = GlobalToGridPos(Input().MousePos(), camera);

	bb.Center = GameToGlobalPos(GetSpritePos(mapPos), camera) + glm::ivec2{ 2, 2 };
	bb.Size = glm::vec2{ static_cast<float>((Consts().TileSize * Consts().Scale) / 2) };

	curbb.Color = ColorRGBA{ 255, 0, 0, 0 };
	if (currentId != entt::null) {
		const GridElem* grid = reg.try_get<GridElem>(currentId);
		if (grid) {
			curbb.Center = GameToGlobalPos(GetSpritePos(grid->Pos), camera) + glm::ivec2{ 2, 2 };
			curbb.Color = ColorRGBA{ 255, 0, 0, 255 };
			curbb.Size = glm::vec2{ static_cast<float>((Consts().TileSize * Consts().Scale) / 2) };
		}
	}
}

void EditorScene::DrawGui() {
	DrawMainGui();
	DrawEntityGui();
	DrawSelectorGui();
	DrawEntityPresetsGui();
}

void EditorScene::DrawMainGui() {
	ImGui::Begin("Level Editor");
	ImGui::InputText("Level Name", &levelName);
	if (ImGui::Button("Load")) {
		std::string tmp = levelName;
		Reset();
		Load(tmp);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save")) {
		SaveLevelData(reg, levelName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear")) {
		ResetMap();
		LoadMap();
	}
	ImGui::End();
}

template<typename T>
using HasEditT = decltype(std::declval<T&>().Edit());

template<typename T>
constexpr bool HasEdit = detect_v<T, HasEditT>::value;


void EditorScene::DrawEntityGui() {
	bool entityDeleted = false;
	ImGui::Begin("Entity Editor");
	if (currentId != entt::null) {
		std::string sid = std::string("Entity: ") + std::to_string(static_cast<uint32_t>(currentId));
		ImGui::Text(sid.c_str());

		GridElem* grid = reg.try_get<GridElem>(currentId);
		if (grid) {
			grid->ClearGridElem(currentId, map);
		}

		if (ImGui::Button("Delete")) {
			entityDeleted = true;
		}
		
		ImGui::Separator();

		int cnt = 0;
		ImGui::Text("Add Components");
		ImGui::Separator();
		ImGui::BeginChild("NewComponentList", { ImGui::GetWindowContentRegionWidth(), 100 }, false, ImGuiWindowFlags_HorizontalScrollbar);
		auto createCallback = [&](auto type) {
			using Comp = typename decltype(type)::type;
			ImGui::PushID(cnt++);
			if (!reg.has<Comp>(currentId) && ImGui::Button(Comp::ComponentName.c_str())) {
				reg.emplace<Comp>(currentId);
			}
			ImGui::PopID();
		};
		ForEachType(EditorCompList(), createCallback);
		ImGui::EndChild();

		ImGui::Separator();

		auto showComponent = [&](auto type) {
			using Comp = typename decltype(type)::type;
			if (!reg.has<Comp>(currentId)) {
				return;
			}
			ImGui::PushID(cnt++);
			if (ImGui::CollapsingHeader(Comp::ComponentName.c_str())) {
				if (ImGui::Button("Remove")) {
					reg.remove<Comp>(currentId);
				}
				else {
					if constexpr (!std::is_empty_v<Comp> && HasEdit<Comp>) {
						reg.get<Comp>(currentId).Edit();
					}
				}
			}
			ImGui::PopID();
		};

		ImGui::Text("Edit Components");
		ImGui::Separator();
		ForEachType(EditorCompList(), showComponent);

		grid = reg.try_get<GridElem>(currentId);
		if (grid) {
			grid->UpdateGridElem(currentId, map);
		}
	}
	ImGui::End();

	if (entityDeleted) {
		GridElem* grid = reg.try_get<GridElem>(currentId);
		if (grid) {
			grid->ClearGridElem(currentId, map);
		}
		reg.destroy(currentId);
		currentId = entt::null;
	}
}

void EditorScene::ShowEntity(entt::entity id) {
	std::string sid = std::string("Entity: ") + std::to_string(static_cast<uint32_t>(id));
	if (ImGui::RadioButton(sid.c_str(), id == currentId)) {
		currentId = id;
	}
}

void EditorScene::DrawSelectorGui() {
	ImGui::Begin("Select Entity");

	ImGui::Text("Mapped entities");
	ImGui::Separator();
	
	ImGui::BeginChild("MapList", { ImGui::GetWindowContentRegionWidth(), 200 }, false, ImGuiWindowFlags_HorizontalScrollbar);
	bool first = true;
	for (const auto& p : map) {
		if (!first) {
			ImGui::Separator();
		}
		first = false;

		ImGui::Text("{ %d, %d }", p.first[0], p.first[1]);
		for (auto id : p.second) {
			ShowEntity(id);
		}
	}
	
	if (!first) {
		ImGui::Separator();
	}

	ImGui::Text("Non mapped entities");
	ImGui::Separator();
	auto view = reg.view<EditorTag>(entt::exclude<GridElem>);
	for (auto id : view) {
		ShowEntity(id);
	}

	ImGui::EndChild();
	
	ImGui::End();
}

void EditorScene::DrawEntityPresetsGui() {
	ImGui::Begin("Entity Presets");

	if (ImGui::Button("Load presets")) {
		std::ifstream presetsFile(Paths::Editor + presetsName);
		if (presetsFile.good()) {
			presetsFile >> entityPresets;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save presets")) {
		std::ofstream file(Paths::Editor + presetsName);
		if (!file.good()) {
			std::cerr << "Can't save file " << Paths::Editor + presetsName << std::endl;
		}
		else {
			file << entityPresets;
		}
	}

	ImGui::Separator();

	if (currentId != entt::null) {
		ImGui::InputText("Preset name", &curPresetName);
		if (ImGui::Button("Save current entity")) {
			nlohmann::json res;
			res["Name"] = curPresetName;
			res["Components"] = SaveEntity(LevelCompList(), reg, currentId);
			entityPresets[curPresetName] = res;
		}
		ImGui::Separator();
	}
	
	ImGui::Text("Presets list");
	ImGui::Separator();
	if (ImGui::RadioButton("Empty", curPreset.is_null())) {
		curPreset = nlohmann::json();
	}
	for (const auto& node : entityPresets) {
		const std::string& name = node.at("Name").get<std::string>();
		bool cur = !curPreset.is_null() && curPreset["Name"] == name;
		if (ImGui::RadioButton(name.c_str(), cur)) {
			curPreset = node;
			curPresetName = name;
		}
	}

	ImGui::End();
}

void EditorScene::Clear() {
	frameBuffer.Select();
	frameBuffer.Clear(RGBA({ 0, 0, 0, 255 }));
}

void EditorScene::Draw() {
	frameBuffer.Select();
	render.Draw(camera);

	DrawFramebufferToScreen(frameBuffer);
	bb.Draw();
	curbb.Draw();
	DrawGui();
}
