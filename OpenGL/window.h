#pragma once
#include "shader.h"
#include "gui.h"

#include "animated_sprite.h"

#include <SDL.h>

class Window
{
	bool open = false;

	SDL_Window* window = nullptr;
	SDL_GLContext glcontext = nullptr;

	glm::uvec2 size = { 800, 600 };

	GUI gui{};

	void LoadSDL();
	void LoadOpenGL();

	void ProcessEvents();
public:
	Window() = default;
    ~Window();

	void Load();
	void Reset();

	void Update();
	void UpdateViewport();

	void Clear();
	void Render();

	bool Open() { return open; }

	glm::uvec2 GetSize() { return size; }
};