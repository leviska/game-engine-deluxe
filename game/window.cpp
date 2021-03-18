#include <window.h>
#include <game.h>
#include <assertion.h>

#include <imgui.h>
#include <input.h>

#include <resources/consts.h>

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
	DOUT() << "OpenGL error: " << message << std::endl;
	static bool ignoreErrors = false;
	if (!ignoreErrors) {
		DebugBreak();
	}
	if (!ignoreErrors) {
		THROWERROR("OpenGL error: " + std::string(message));
	}
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

	window = SDL_CreateWindow("RPG Quest", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, 
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
	//glEnable(GL_DEPTH_TEST);
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

	ConstsMut().WindowSize = size;

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
		InputMut().Update(event);

		switch (event.type) {
		case SDL_QUIT: {
			open = false;
			break;
		}
		case SDL_WINDOWEVENT: {
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				SDL_GL_GetDrawableSize(window, (int*)&size.x, (int*)&size.y);
				UpdateWindowSize(size);
			}
			break;
		}
		}
	}
}

void Window::ProcessKeys() {
	if (Input().KeyPressed(Keyboard::F11)) {
		fullscreen = !fullscreen;
		SDL_SetWindowFullscreen(window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	}
}

void Window::Update() {
	InputMut().Tick();
	ProcessEvents();
	ProcessKeys();
}

void Window::Clear() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gui.Clear();
}

void Window::Render() {
	gui.Render();
	SDL_GL_SwapWindow(window);
}

bool Window::Open() {
	return open;
}
