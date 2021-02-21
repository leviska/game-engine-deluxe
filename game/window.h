#pragma once
#include "shader.h"
#include "gui.h"

#include <sdl2/SDL.h>

class Window
{
	SDL_Window* window = nullptr;
	SDL_GLContext glcontext = nullptr;

	bool open = false;
	bool fullscreen = false;
	glm::uvec2 size = { 800, 600 };
	glm::vec4 clearColor;

	GUI gui;

	void LoadSDL();
	void LoadOpenGL();

	void ProcessEvents();
	void ProcessKeys();
public:
	Window() = default;
    ~Window();

	void Load();
	void Reset();

	void Update();
	void UpdateViewport();

	void Clear();
	void Render();

	bool Open();

	void SetClearColor(const glm::vec4& color);

	glm::uvec2 GetSize();
};
