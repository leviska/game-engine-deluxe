#pragma once

#include <sdl2/SDL.h>

class GUI {
	SDL_Window* window;
public:
	void Load(SDL_Window* window, SDL_GLContext glcontext);
	void Reset();

	void ProcessEvents(const SDL_Event& event);
	
	void Clear();
	void Render();
};
