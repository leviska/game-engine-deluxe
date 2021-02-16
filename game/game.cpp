#include "game.h"

#include "imgui.h"
#include "resources.h"

#include <iostream>
#include <chrono>

GameInst& GameInst::GetInstance() {
	static GameInst inst;
	return inst;
}

GameInst& Game() {
	return GameInst::GetInstance();
}

Scene& GameInst::GetCurrentScene() {
	return scene;
}

uint32_t GameInst::GetScale() {
	glm::uvec2 scale = window.GetSize() / Resources().CanvasSize;
	return std::min(scale.x, scale.y);
}

void GameInst::Load() {
	window.Load();
	if (leOpen) {
		leScene.Load();
	}
	else {
		scene.Load();
	}
}

void GameInst::Run() {
	Load();
	while (running) {
		Update();
		
		Clear();
		Draw();
		Render();
	}
}

void GameInst::Update() {
	fps.Update();
	fps.LimitFPS(60);

	window.Update();

	if (window.F2Pressed) {
		if (!leOpen) {
			scene.Reset();
			leScene.Load();
		}
		else {
			leScene.Reset();
			scene.Load();
		}
		leOpen = !leOpen;
	}

	if (!leOpen)
		scene.Update();
	else
		leScene.Update();

	if (resetResources) {
		Resources().Reset();
		Resources().Load();

		window.UpdateViewport();

		resetResources = false;
	}
	running = running && window.Open();
}

void GameInst::Clear() {
	window.Clear();
	if (!leOpen)
		scene.Clear();
	else
		leScene.Clear();
}

void GameInst::Draw() {
	if (!leOpen)
		scene.Draw();
	else
		leScene.Draw();

	ImGui::Begin("Resources info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	resetResources = ImGui::Button("Reload");
	ImGui::End();

	fps.Draw();
}

void GameInst::Render() {
	window.Render();
}

void GameInst::UpdateViewport(glm::vec2 size) {
	glViewport(0, 0, size.x, size.y);
	for (auto id : ShadersArray) {
		Resources().GetShader(id).Select();
		Resources().GetShader(id).UpdateProjection(size.x, (id == Shaders::Buffer || id == Shaders::Shapes ? -size.y : size.y));
	}
}