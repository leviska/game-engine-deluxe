#include "level_editor.h"

#include "renderable.h"
#include "game.h"
#include "input.h"
#include "utility.h"

#include "consts.h"
#include "graphics.h"
#include "shaders.h"

#include <fstream>
#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <sdl2/SDL.h>


void LevelEditorScene::Load(const std::string& name) {
	levelName = name;

	glm::vec2 halfCanvas(Consts().CanvasSize / 2u);
	camera = Camera{ halfCanvas, halfCanvas };

	frameBuffer.Load(Consts().CanvasSize);

	try {
		LoadMap(map, db, render, name);
	}
	catch (std::runtime_error exp) {
		// can't find file, ignore
	}

	SpritePtr sptr = render.Stage(Graphics().EditorButtons[currentTile]);
	preview = std::make_unique<SpriteOwner>(sptr);
}

void LevelEditorScene::Reset() {
	frameBuffer.Reset();
	map.clear();
	db.clear();
	preview.reset();
	render.Reset();
	levelName.clear();
}

const std::string& LevelEditorScene::CurrentLevel() const {
	return levelName;
}

void LevelEditorScene::OnLeftPress(glm::ivec2 relPos) {
	entt::entity id;
	id = CreateElement(relPos, map, db);

	const std::string& buttonName = Graphics().EditorButtons[currentTile].Name;
	if (buttonName == "FrontWall") {
		db.emplace<FrontWall>(id);
	}
	else if (buttonName == "Tiling") {
		db.emplace<TilingMap>(id);
	}
	else if (buttonName == "Floor") {
		db.emplace<Floor>(id);
	}
	else if (buttonName == "Player") {
		db.emplace<Player>(id);
	}
	else if (buttonName == "Orc") {
		db.emplace<Orc>(id);
	}
	else if (buttonName == "Ladder") {
		db.emplace<Ladder>(id);
	}

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			UpdateSprite(relPos + glm::ivec2(i, j), map, db, render);
		}
	}
}

void LevelEditorScene::OnRightPress(glm::ivec2 relPos) {
	auto it = map.find(relPos);
	if (it != map.end()) {
		for (auto id : it->second) {
			db.destroy(id);
		}
		it->second.clear();
	}
	
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			UpdateSprite(relPos + glm::ivec2(i, j), map, db, render);
		}
	}
}

void LevelEditorScene::Update() {
	camera.UpdateFreeCamera();

	if (Input().MouseCaptured())
		return;
	glm::ivec2 mousePos;
	SDL_GetMouseState(&(mousePos.x), &(mousePos.y));
	glm::ivec2 centerShift = Game().GetWindow().GetSize() / 2u - Consts().CanvasSize * Game().GetScale() / 2u;
	glm::ivec2 cameraShift = glm::ivec2(camera.Size - camera.Pos) * to_i32(Game().GetScale());
	glm::ivec2 gamePos = mousePos - cameraShift - centerShift;
	int32_t scaledTile = Consts().TileSize * Game().GetScale();
	glm::ivec2 relPos = glm::floor(glm::vec2(gamePos) / glm::vec2(scaledTile));
	glm::ivec2 camShift = (scaledTile + cameraShift + centerShift) % scaledTile;
	glm::ivec2 screenPos = ((mousePos - camShift) / scaledTile * scaledTile + camShift);

	bb.Center = screenPos + glm::ivec2{ scaledTile / 2, scaledTile / 2 };
	bb.Size = glm::vec2{ scaledTile / 2, scaledTile / 2 };
	bb.Color = ColorRGBA{ 255, 255, 255, 255 };

	(**preview) = Graphics().EditorButtons[currentTile].Value;
	(**preview).Pos = GetSpritePos(relPos);

	if (Input().KeyPressed(Mouse::Left) || Input().KeyDown(Mouse::Left) && prevPos != relPos) {
		OnLeftPress(relPos);
	}
	if (Input().KeyPressed(Mouse::Right) || Input().KeyDown(Mouse::Right) && prevPos != relPos) {
		OnRightPress(relPos);
	}
	prevPos = relPos;
}

bool DrawTileButton(const SpriteInfo& info) {
	ImTextureID textId = reinterpret_cast<void*>(static_cast<intptr_t>(Graphics().Textures["EditorButtons2D"].GetId()));
	glm::vec2 textSize(Graphics().Textures["EditorButtons2D"].GetSize());
	glm::vec2 sizeV = info.Value.TextSize * 4.0f;
	glm::vec2 uv0V = info.Value.TextPos;
	glm::vec2 uv1V = info.Value.TextPos + info.Value.TextSize;
	uv0V /= textSize;
	uv1V /= textSize;
	ImVec2 size{ sizeV.x, sizeV.y };
	ImVec2 uv0{ uv0V.x, uv0V.y };
	ImVec2 uv1{ uv1V.x, uv1V.y };
	ImVec4 bg_col = ImVec4(31.0f / 255.0f, 14.0f / 255.0f, 28.0f / 255.0f, 1.0f);
	return ImGui::ImageButton(textId, size, uv0, uv1, 1, bg_col);
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
		SaveMap(map, db, levelName);
	}
	
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 oldSpacing = style.ItemSpacing;
	style.ItemSpacing = { 0, 0 };
	
	const NamedVector<SpriteInfo>& info = Graphics().EditorButtons;
	uint32_t currentLineDrawed = 0;
	bool firstDraw = true;
	for (size_t i = 0; i < info.Size(); i++) {
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
			currentTile = i;
		}
			
		ImGui::PopID();
	}

	style.ItemSpacing = oldSpacing;
	
	ImGui::Text("Current Tile");
	DrawTileButton(info[currentTile]);
	ImGui::End();
}

void LevelEditorScene::Clear() {
	frameBuffer.Select();
	frameBuffer.Clear(RGBA({ 0, 0, 0, 255 }));
}

void LevelEditorScene::Draw() {
	frameBuffer.Select();
	render.Draw(camera);
	
	DrawFramebufferToScreen(frameBuffer);
	bb.Draw();
	DrawGui();
}
