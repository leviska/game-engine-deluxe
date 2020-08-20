#include "image.h"

#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

glm::vec4 RGBA(glm::u8vec4 rgba) {
	return glm::vec4(rgba) / 255.0f;
}

void Image::Load(glm::uvec2 Size) {
	size = Size;
	image.resize(size.x * size.y);
}

void Image::Load(std::string file) {
	SDL_Surface* surface = IMG_Load(file.c_str());

	if (!surface) {
		throw std::runtime_error("Cannot load " + file + " image file");
	}

	Load(glm::uvec2{ surface->w, surface->h });

	for (size_t i = 0; i < size.x * size.y; i++) {
		uint32_t pixel = *(static_cast<uint32_t*>(surface->pixels) + i);
		SDL_GetRGBA(pixel, surface->format, &image[i].r, &image[i].g, &image[i].b, &image[i].a);
	}
	SDL_FreeSurface(surface);
}

void Image::Reset() {
	size = { 0, 0 };
	image.clear();
}


glm::u8vec4& Image::operator[](glm::uvec2 pos) {
	return image[pos.y * size.x + pos.x];
}

const glm::u8vec4& Image::operator[](glm::uvec2 pos) const {
	return image[pos.y * size.x + pos.x];
}
