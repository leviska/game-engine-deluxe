#include "game.h"

#include "imgui.h"
#include "input.h"
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

FPSInfo GameInst::FPS() {
	return fps.LastFrame();
}

SceneController& GameInst::GetSceneController() {
	return scenes;
}

Window& GameInst::GetWindow() {
	return window;
};


uint32_t GameInst::GetScale() {
	glm::uvec2 scale = window.GetSize() / Resources().CanvasSize;
	return std::min(scale.x, scale.y);
}

void GameInst::Load() {
	window.Load();
	scenes.Load();
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
	fps.LimitFPS(0);

	window.Update();

	if (Input().KeyPressed(Keyboard::F2)) {
		Scenes scene = scenes.CurrentState() == Scenes::Sandbox ? Scenes::LevelEditor : Scenes::Sandbox;
		scenes.SceneState().ChangeState(static_cast<int32_t>(scene));
	}

	scenes.CurrentScene().Update();

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
	scenes.CurrentScene().Clear();
}

void GameInst::Draw() {
	scenes.CurrentScene().Draw();

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
