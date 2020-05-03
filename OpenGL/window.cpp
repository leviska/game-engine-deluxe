#include "window.h"

#include <cassert>
#include <iostream>

#include <glad/glad.h>
#include <windows.h>

const float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
const unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

Window::Window()
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return;
	}

	glcontext = SDL_GL_CreateContext(window);
	if (!glcontext) {
		std::cerr << "Unable to create GL context: " << SDL_GetError() << std::endl;
		return;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// OpenGL
	glViewport(0, 0, 800, 600);

	// Shaders
	shader.Init();
	if (!shader.Load("vertex.glsl", "fragment.glsl")) {
		std::cerr << "Failed to load shaders" << std::endl;
		return;
	}

	// Triangle
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	good = true;
	open = true;
}

Window::~Window()
{
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	if (glcontext)
		SDL_GL_DeleteContext(glcontext);

	if (window)
		SDL_DestroyWindow(window);

	SDL_Quit();
}

void Window::Process()
{
	assert(good);
	SDL_Event event;
	while (open) {
		while (SDL_PollEvent(&event) != 0) {
			//User requests quit
			if (event.type == SDL_QUIT) {
				open = false;
			}
			if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					glViewport(0, 0, event.window.data1, event.window.data2);
				}
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		shader.Select();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader.Set("shift", 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		shader.Set("shift", 0.5f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glFlush();
		SDL_GL_SwapWindow(window);
	}
}