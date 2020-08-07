#include "level_editor.h"

#include "renderable.h"

#include <fstream>
#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <SDL.h>


void LevelEditorScene::Load(std::string name) {
	levelName = name;
	tileRender.Load(0);
	map.Load(db);

	std::string fileName = std::string("assets/") + levelName + std::string(".json");
	std::ifstream file(fileName);
	if (file.good()) {
		nlohmann::json level;
		file >> level;
		LoadMap(map, level);
	}
}

void LevelEditorScene::Reset() {
	tileRender.Reset();
	db.clear();
	map.Reset();
	levelName.clear();
}


bool IsWallName(const std::string& str) {
	const std::string& prefix = "Wall";
	for (size_t k = 0; k < prefix.size(); k++) {
		if (str[k] != prefix[k])
			return false;
	}
	return true;
}

bool DrawTileButton(const SpriteInfo& info) {
	ImTextureID textId = (void*)Resources().GetTexture(1).GetId();
	glm::vec2 textSize(Resources().GetTexture(1).GetSize());
	glm::vec2 sizeV = info.Size * 4.0f;
	glm::vec2 uv0V = info.Position;
	glm::vec2 uv1V = info.Position + info.Size;
	uv0V /= textSize;
	uv1V /= textSize;
	ImVec2 size{ sizeV.x, sizeV.y };
	ImVec2 uv0{ uv0V.x, uv0V.y };
	ImVec2 uv1{ uv1V.x, uv1V.y };
	ImVec4 bg_col = ImVec4(39.0f / 255.0f, 39.0f / 255.0f, 68.0f / 255.0f, 1.0f);
	return ImGui::ImageButton(textId, size, uv0, uv1, 1, bg_col);
}

void LevelEditorScene::Update() {
	if (ImGui::GetIO().WantCaptureMouse)
		return;
	glm::ivec2 mousePos;
	uint32_t buttons = SDL_GetMouseState(&(mousePos.x), &(mousePos.y));
	glm::ivec2 relPos = mousePos / static_cast<int32_t>(Resources().TileSize);
	bb.Center = relPos * static_cast<int32_t>(Resources().TileSize) + glm::ivec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
	bb.Size = glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
	bb.Color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f };
	if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Sprite sprite;
		sprite.Scale = Resources().Scale;
		sprite.Pos = glm::vec2{ relPos.x * Resources().TileSize, relPos.y * Resources().TileSize } +glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
		sprite.Load(data.CurrentTile);
		map.SetWall(relPos, { sprite });
	}
	if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		if (map.Has(relPos)) {
			entt::entity id = map.Get(relPos);
			db.destroy(id);
			map.Update(entt::null, relPos);
		}
	}
}

void LevelEditorScene::DrawGui() {
	ImGui::Begin("Level Editor", nullptr);
	ImGui::InputText("Level Name", &levelName);
	if (ImGui::Button("Load")) {
		std::string tmp = levelName;
		Reset();
		Load(tmp);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save")) {
		nlohmann::json result;
		SaveMap(map, result);
		std::string fileName = "assets/" + levelName + ".json";
		std::ofstream file(fileName);
		if (!file.good()) {
			std::cerr << "Cannot open " + levelName + " for writing" << std::endl;
		}
		else {
			file << result;
		}
	}
	const NamedVector<SpriteInfo>& info = Resources().GetSpriteInfoLE();
	uint32_t currentLineDrawed = 0;
	bool firstDraw = true;
	for (size_t i = 0; i < info.Size(); i++) {
		if (IsWallName(info[i].Name)) {
			ImGui::PushID(i);
			if (currentLineDrawed >= 3) {
				currentLineDrawed = 0;
			}
			else if (!firstDraw) {
				currentLineDrawed++;
				ImGui::SameLine();
			}
			firstDraw = false;
			if (DrawTileButton(info[i])) {
				data.CurrentTile = info[i].Name;
			}
			ImGui::PopID();
		}
	}
	ImGui::Text("Current Tile");
	DrawTileButton(info[data.CurrentTile]);
	ImGui::End();
}

void LevelEditorScene::Clear() {
	tileRender.Clear();
}

void LevelEditorScene::Draw() {
	DrawGui();
	RenderSystem(db, tileRender);
	tileRender.Draw();
	bb.Draw();
}
