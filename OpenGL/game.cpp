#include "game.h"

#include "imgui.h"

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

void GameInst::Load() {
	window.Load();
	scene.Load("untitled");
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
	scene.Update();

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
	scene.Clear();
}

void GameInst::Draw() {
	scene.Draw();

	ImGui::Begin("Resources info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	resetResources = ImGui::Button("Reload");
	ImGui::End();

	fps.Draw();
}

void GameInst::Render() {
	window.Render();
}
