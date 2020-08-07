#pragma once

#include "window.h"
#include "fps.h"
#include "level_editor.h"

class GameInst {
	Window window;
	FPS fps;
	LevelEditorScene scene;

	bool running = true;

	bool resetResources = false;
public:
	GameInst(const GameInst&) = delete;
	GameInst& operator=(const GameInst&) = delete;

	static GameInst& GetInstance();

	void Run();

	FPSInfo FPS() { return fps.LastFrame(); }
	Scene& GetCurrentScene();
private:
	GameInst() = default;
	~GameInst() = default;

	void Load();
	void Update();
	void Clear();
	void Draw();
	void Render();
};

GameInst& Game();