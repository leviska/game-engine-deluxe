#include "image.h"

#include "assertion.h"

#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>

glm::vec4 RGBA(ColorType rgba) {
	return glm::vec4(rgba) / 255.0f;
}

ColorType ColorAlpha(ColorChannel alpha) {
	return ColorType{ 255, 255, 255, alpha };
}
void Image::Load(glm::uvec2 Size, ColorType DefaultValue) {
	size = Size;
	image.resize(size.x * size.y, DefaultValue);
}

void Image::Load(const std::string& file) {
	SDL_Surface* surface = IMG_Load(file.c_str());

	if (!surface) {
		THROWERROR("Cannot load " + file + " image file");
	}

	Load(glm::uvec2{ surface->w, surface->h });

	for (size_t i = 0; i < size.x * size.y; i++) {
		uint32_t pixel = *(static_cast<uint32_t*>(surface->pixels) + i);
		SDL_GetRGBA(pixel, surface->format, &image[i].r, &image[i].g, &image[i].b, &image[i].a);
	}
	SDL_FreeSurface(surface);
}

void Image::Save(const std::string& file) {
	SDL_Surface* surface = SDL_CreateRGBSurface(0, size.x, size.y, 32, 0xff000000, 0xff0000, 0xff00, 0xff);
	for (size_t i = 0; i < size.x * size.y; i++) {
		uint32_t pixel = SDL_MapRGBA(surface->format, image[i].r, image[i].g, image[i].b, image[i].a);
		*(static_cast<uint32_t*>(surface->pixels) + i) = pixel;
	}
	IMG_SavePNG(surface, file.c_str());
	SDL_FreeSurface(surface);
}

void Image::Reset() {
	size = { 0, 0 };
	image.clear();
}


ColorType& Image::operator[](glm::uvec2 pos) {
	return image[pos.y * size.x + pos.x];
}

const ColorType& Image::operator[](glm::uvec2 pos) const {
	return image[pos.y * size.x + pos.x];
}
