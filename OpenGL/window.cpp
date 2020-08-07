#include "window.h"
#include "resources.h"

#include <cassert>
#include <stdexcept>

#include <glad/glad.h>
#include <SDL_image.h>
#include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Window::~Window() {
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
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	while (SDL_PollEvent(&event) != 0) {
		gui.ProcessEvents(event);
		if (event.type == SDL_QUIT) {
			open = false;
		}
		if (event.type == SDL_WINDOWEVENT) {
			if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
				width = event.window.data1;
				height = event.window.data2;
				UpdateViewport();
			}
		}
	}
}

void Window::UpdateViewport() {
	glViewport(0, 0, width, height);
	for (uint32_t i = 0; i < static_cast<uint32_t>(Shaders::Total); i++) {
		Resources().GetShader(i).Select();
		Resources().GetShader(i).UpdateProjection(static_cast<float>(width), static_cast<float>(height));
	}
}

void Window::Update() {
	ProcessEvents();
}

void Window::Clear() {
	glClearColor(39.0f / 255.0f, 39.0f / 255.0f, 68.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	gui.Clear();
}

void Window::Render() {
	gui.Render();
	glFlush();
	SDL_GL_SwapWindow(window);
}
