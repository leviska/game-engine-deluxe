#pragma once
#include "shader.h"

#include <SDL.h>

class Window
{
public:
    Window();
    ~Window();

	bool IsGood() { return good; }

    void Process();
private:
	bool good = false;
	bool open = false;
	
	SDL_Window* window = nullptr;
	SDL_GLContext glcontext = nullptr;

	Shader shader;

	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;
};