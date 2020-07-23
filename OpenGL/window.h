#pragma once
#include "shader.h"
#include "utility.h"
#include "gui.h"
#include "batching.h"
#include "particles.h"

#include "animated_sprite.h"

#include <SDL.h>

class Window
{
	bool open = false;

	SDL_Window* window = nullptr;
	SDL_GLContext glcontext = nullptr;

	uint32_t width = 800;
	uint32_t height = 600;

	BatchedRender render;
	BatchedRender render2;
	ParticleRender particle;
	ParticleRender particle2;
	ParticleRender particle3;

	FPS fps;
	GUI gui;

	void LoadSDL();
	void LoadOpenGL();

	void ProcessEvents();

	void Update();

	void Clear();
	void Draw();
	void Render();
public:
	Window() = default;
    ~Window();

	void Load();
	void Reset();

    void Run();
};