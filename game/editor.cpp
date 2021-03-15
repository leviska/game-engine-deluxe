#include "editor.h"

#include "renderable.h"
#include "input.h"
#include "utility.h"
#include "level.h"
#include "serialize_components.h"
#include "components_utility.h"
#include "components_list.h"

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


void EditorScene::Load(const std::string& name) {
	levelName = name;
	render.Load();

	glm::vec2 halfCanvas(Consts().CanvasSize / 2u);
	camera = Camera{ halfCanvas, halfCanvas };

	frameBuffer.Load(Consts().CanvasSize);

	try {
		LoadLevel(levelName, reg, render, map);
	}
	catch (std::runtime_error exp) {
		// can't find file, ignore
	}

	bb.Color = ColorRGBA{ 255, 255, 255, 255 };
}

void EditorScene::Reset() {
	frameBuffer.Reset();
	map.clear();
	reg.clear();
	render.Reset();
	levelName.clear();

	currentId = entt::null;
	selectorPos = { 0, 0 };
}

const std::string& EditorScene::CurrentLevel() const {
	return levelName;
}

void EditorScene::Update() {
	camera.UpdateFreeCamera();

	glm::ivec2 mapPos = GlobalToGridPos(Input().MousePos(), camera);

	bb.Center = GameToGlobalPos(GetSpritePos(mapPos), camera) + glm::ivec2{ 2, 2 };
	bb.Size = glm::vec2{ static_cast<float>((Consts().TileSize * Consts().Scale) / 2) };

	if (Input().KeyPressed(Mouse::Left)) {
		if (Input().KeyDown(Keyboard::LSHIFT)) {
			currentId = reg.create();
			map[mapPos].push_back(currentId);

			reg.emplace<SimpleSpriteData>(currentId);
			reg.emplace<GridElem>(currentId, mapPos);

			SpritePtr sptr = render.Stage("Square");
			sptr->Pos = GetSpritePos(mapPos);
			reg.emplace<Renderable>(currentId).push_back(sptr);
		}
		else {
			selectorPos = mapPos;
		}
	}
	if (Input().KeyPressed(Mouse::Right)) {

	}
}

void EditorScene::DrawGui() {
	DrawMainGui();
	DrawEntityGui();
	DrawSelectorGui();
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
	ImGui::End();
}

void EditorScene::DrawEntityGui() {
	ImGui::Begin("Entity Editor");
	if (currentId != entt::null) {
		std::string sid = std::string("Entity: ") + std::to_string(static_cast<uint32_t>(currentId));
		ImGui::Text(sid.c_str());
		/*
		if (ImGui::Button("AddComponent")) {
			auto createCallback = [&](auto& comp) {
				if (ImGui::Button(comp.ComponentName.c_str())) {
					reg.emplace<decltype(comp)>()
				}
			};
			CallForEveryComp(reg, currentId, callEdit, EditorCompList());
		}
		*/
		auto callEdit = [](auto& comp) {
			if (ImGui::CollapsingHeader(comp.ComponentName.c_str())) {
				comp.Edit();
			}
		};
		CallForEveryComp(reg, currentId, callEdit, EditorCompList());
		if (Graphics().Sprites.Has(reg.get<SimpleSpriteData>(currentId).Name)) {
			Renderable& rend = reg.get<Renderable>(currentId);
			rend.clear();
			SpritePtr sptr = render.Stage(reg.get<SimpleSpriteData>(currentId).Name);
			sptr->Pos = GetSpritePos(reg.get<GridElem>(currentId).Pos);
			rend.push_back(sptr);
		}
	}
	ImGui::End();
}

void EditorScene::DrawSelectorGui() {
	ImGui::Begin("Select Entity");
	if (map.find(selectorPos) != map.end()) {
		for (auto id : map[selectorPos]) {
			std::string sid = std::string("Entity: ") + std::to_string(static_cast<uint32_t>(id));
			if (ImGui::Button(sid.c_str())) {
				currentId = id;
			}
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
	DrawGui();
}
