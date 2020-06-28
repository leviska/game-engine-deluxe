#include "Texture.h"

#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>

void Texture::Load(const std::string& name) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_RECTANGLE, id);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	SDL_Surface* surface = IMG_Load(name.c_str());

	if (!surface) {
		throw std::runtime_error("Cannot load " + name + " image file");
	}

	width = surface->w;
	height = surface->h;

	int Mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, Mode, width, height, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);
}

void Texture::Reset() {
	glDeleteTextures(1, &id);
}

void Texture::Select() const {
	glBindTexture(GL_TEXTURE_RECTANGLE, id);
}


uint32_t Texture::GetId() const {
	return id;
}

uint32_t Texture::GetWidth() const {
	return width;
}

uint32_t Texture::GetHeight() const {
	return height;
}
