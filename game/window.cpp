#include "window.h"
#include "resources.h"
#include "game.h"
#include "assertion.h"

#include "imgui.h"

#include <cassert>
#include <stdexcept>
#include <iostream>

#include <glad/glad.h>
#include <sdl2/SDL_image.h>
#include <windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void GLAPIENTRY GlErrorHandler(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cout << "OpenGL error: " << message << std::endl;
}

Window::~Window() {
	Reset();
}

void Window::LoadSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		THROWERROR("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
	}

	int imgFlags = IMG_INIT_PNG;
	if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
		THROWERROR("SDL_image could not initialize! SDL_Error: " + std::string(IMG_GetError()));
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window) {
		THROWERROR("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
	}

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
	
	glcontext = SDL_GL_CreateContext(window);
	if (!glcontext) {
		THROWERROR("Unable to create GL context: " + std::string(SDL_GetError()));
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		THROWERROR("Failed to initialize GLAD");
	}
}

void Window::LoadOpenGL() {
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlErrorHandler, 0);
#endif
}

void Window::Load() {
	LoadSDL();
	LoadOpenGL();

	Resources().Load();

	UpdateViewport();

	gui.Load(window, glcontext);

	SDL_MaximizeWindow(window);
	open = true;
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
	F2Pressed = false;
	PressedMouse1 = false;
	PressedMouse2 = false;
	while (SDL_PollEvent(&event) != 0) {
		gui.ProcessEvents(event);
		switch (event.type) {
		case SDL_QUIT: {
			open = false;
			break;
		}
		case SDL_WINDOWEVENT: {
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				SDL_GL_GetDrawableSize(window, (int*)&size.x, (int*)&size.y);
				UpdateViewport();
			}
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_F11) {
				fullscreen = !fullscreen;
				SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
			}
			else if (event.key.keysym.sym == SDLK_F2) {
				F2Pressed = true;
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN: {
			if (!ImGui::GetIO().WantCaptureMouse) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					PressedMouse1 = true;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					PressedMouse2 = true;
				}
			}
			break;
		}
		}
	}
}

void Window::UpdateViewport() {
	Game().UpdateViewport({ size.x, size.y });
}

void Window::Update() {
	ProcessEvents();
}

void Window::Clear() {
	//glClearColor(39.0f / 255.0f, 39.0f / 255.0f, 68.0f / 255.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	gui.Clear();
}

void Window::Render() {
	gui.Render();
	SDL_GL_SwapWindow(window);
}
