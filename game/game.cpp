#include "game.h"

#include "imgui.h"
#include "input.h"
#include "utility.h"

#include "glbuffers.h"
#include "graphics.h"
#include "paths.h"
#include "consts.h"
#include "shaders.h"
#include "editor_info.h"
#include "assertion.h"

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


void GameInst::Load() {
	window.Load();
	Reload();
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

	if (Input().KeyPressed(Keyboard::F1)) {
		Scenes scene = scenes.SceneState().State<Scenes>() == Scenes::Editor ? Scenes::Level : Scenes::Editor;
		if (scene == Scenes::Level) {
			scenes.Level().Reset();
			scenes.Level().Load(scenes.Editor().CurrentLevel());
		}
		scenes.SceneState().ChangeState(to_i32(scene));
	}

	if (Input().KeyPressed(Keyboard::F2)) {
		Scenes scene = scenes.SceneState().State<Scenes>() == Scenes::Sandbox ? Scenes::Level : Scenes::Sandbox;
		scenes.SceneState().ChangeState(scene);
	}

	scenes.CurrentScene().Update();

	if (resetResources) {
		Reload();
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

void GameInst::Reload() {
	DOUT() << "========================" << std::endl;
	DOUT() << "Reloading all the assets" << std::endl;

	BuffersMut().Reset();
	GraphicsMut().Reset();
	ConstsMut().Reset();
	ShadersMut().Reset();
	EditorInfoMut().Reset();

	BuffersMut().Load();
	GraphicsMut().Load();
	ConstsMut().Load();
	ShadersMut().Load();
	EditorInfoMut().Load();

	int32_t cur = scenes.SceneState().State();
	scenes.Reset();
	scenes.Load();
	if (cur >= 0) {
		scenes.SceneState().ChangeState(cur);
	}

	DOUT() << "========================" << std::endl;
}