#include "Texture.h"

#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>

Texture::Texture(Texture&& other) noexcept {
	id = other.id;
	size = other.size;
	type = other.type;
	other.id = -1;
	other.size = { 0, 0 };
	other.type = GL_TEXTURE_2D;
}

Texture& Texture::operator=(Texture&& other) {
	id = other.id;
	size = other.size;
	type = other.type;
	other.id = -1;
	other.size = { 0, 0 };
	other.type = GL_TEXTURE_2D;
	return *this;
}

void Texture::CreateAndLoad(void* data, int mode) {
	glGenTextures(1, &id);
	glBindTexture(type, id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(type, 0, mode, size.x, size.y, 0, mode, GL_UNSIGNED_BYTE, data);
}

void Texture::Load(glm::uvec2 Size, bool rectangle) {
	type = (rectangle ? GL_TEXTURE_RECTANGLE : GL_TEXTURE_2D);
	size = Size;
	CreateAndLoad(nullptr, GL_RGBA);
}

void Texture::Load(const std::string& name, bool rectangle) {
	type = (rectangle ? GL_TEXTURE_RECTANGLE : GL_TEXTURE_2D);

	SDL_Surface* surface = IMG_Load(name.c_str());

	if (!surface) {
		throw std::runtime_error("Cannot load " + name + " image file");
	}

	size.x = surface->w;
	size.y = surface->h;

	int mode = GL_RGB;
	if (surface->format->BytesPerPixel == 4) {
		mode = GL_RGBA;
	}

	CreateAndLoad(surface->pixels, mode);

	SDL_FreeSurface(surface);
}

void Texture::Reset() {
	size = { 0, 0 };
	type = GL_TEXTURE_2D;
	glDeleteTextures(1, &id);
}

void Texture::Select(uint32_t num) const {
	glActiveTexture(GL_TEXTURE0 + num);
	glBindTexture(type, id);
}
