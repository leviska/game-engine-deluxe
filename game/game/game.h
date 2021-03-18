#pragma once

#include <engine/fps.h>

#include <game/scene_controller.h>

#include <graphics/window.h>

class GameInst {
	Window window;
	FPS fps;
	SceneController scenes;
	
	bool running = true;
	bool resetResources = false;
public:
	GameInst(const GameInst&) = delete;
	GameInst& operator=(const GameInst&) = delete;

	static GameInst& GetInstance();

	void Run();

	FPSInfo FPS();
	SceneController& GetSceneController();
	Window& GetWindow();
private:
	GameInst() = default;
	~GameInst() = default;

	void Load();
	void Update();
	void Clear();
	void Draw();
	void Render();

	void Reload();
};

GameInst& Game();
