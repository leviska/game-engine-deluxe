#include "window.h"
#include "resources.h"

#include <cassert>
#include <stdexcept>
#include <chrono>
#include <iostream>

#include <glad/glad.h>
#include <SDL_image.h>
#include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Window::~Window()
{
	Reset();
}

void Window::LoadSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
	}

	int imgFlags = IMG_INIT_PNG;
	if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		throw std::runtime_error("SDL_image could not initialize! SDL_Error: " + std::string(IMG_GetError()));
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
	}

	glcontext = SDL_GL_CreateContext(window);
	if (!glcontext) {
		throw std::runtime_error("Unable to create GL context: " + std::string(SDL_GetError()));
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
		return;
	}
}

void Window::LoadOpenGL() {
	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::Load() {
	LoadSDL();
	LoadOpenGL();

	Resources().Load();
	Resources().GetShader(0).Select();
	Resources().GetShader(0).UpdateProjection(width, height);
	Resources().GetShader(1).Select();
	Resources().GetShader(1).UpdateProjection(width, height);

	gui.Load(window, glcontext);

	SDL_MaximizeWindow(window);
	open = true;

	sprites.Load();
	sprites.Sprites().resize(100000);
}

void Window::Reset() {
	if (glcontext)
		SDL_GL_DeleteContext(glcontext);

	if (window)
		SDL_DestroyWindow(window);

	SDL_Quit();
	open = false;
}

void Window::ProcessEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			open = false;
		}
		if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				width = event.window.data1;
				height = event.window.data2;
				glViewport(0, 0, width, height);
				Resources().GetShader(0).Select();
				Resources().GetShader(0).UpdateProjection(width, height);
				Resources().GetShader(1).Select();
				Resources().GetShader(1).UpdateProjection(width, height);
			}
		}
	}
}

void Window::Clear() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	gui.Clear();
}

void Window::Render() {
	gui.Render();
	glFlush();
	SDL_GL_SwapWindow(window);
}

void Window::Draw() {
	sprites.Sprites()[0].Load(1);
	sprites.Sprites()[0].PrepareForDraw();
	for (int i = 0; i < 1000; i++) {
		sprites.Sprites()[i].Load(rand() % 3 + 1);
		sprites.Sprites()[i].SetColor(glm::vec4(rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX, rand() * 1.0f / RAND_MAX, 1.0));
		sprites.Sprites()[i].SetRotation(rand() * 3.14f / RAND_MAX);
		sprites.Sprites()[i].SetPos({ rand() % width, rand() % height });
		sprites[i].Draw();
	}
	sprites.Sprites()[0].ClearAfterDraw();
	//sprites.Update();
	//sprites.Draw();

	gui.DrawDebugInfo(fps.Update());
}

void Window::Run() {
	while (open) {
		ProcessEvents();
		Clear();
		Draw();
		Render();
	}
}