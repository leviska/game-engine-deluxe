#pragma once

#include "window.h"
#include "fps.h"
#include "level_editor.h"
#include "sandbox_scene.h"

class GameInst {
	Window window;
	FPS fps;
	
	bool running = true;
	bool resetResources = false;
public:
	GameInst(const GameInst&) = delete;
	GameInst& operator=(const GameInst&) = delete;

	static GameInst& GetInstance();

	void Run();

	FPSInfo FPS() { return fps.LastFrame(); }
	Scene& GetCurrentScene();
	Window& GetWindow() { return window; };
	uint32_t GetScale();

	void UpdateViewport(glm::vec2 size);
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
