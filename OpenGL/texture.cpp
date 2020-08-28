#include "Texture.h"

#include <glad/glad.h>

uint32_t Texture::currentId = 0;

Texture::Texture(Texture&& other) noexcept {
	id = other.id;
	size = other.size;
	type = other.type;
	other.id = -1;
	other.size = { 0, 0 };
	other.type = GL_TEXTURE_2D;
}

Texture& Texture::operator=(Texture&& other) noexcept {
	id = other.id;
	size = other.size;
	type = other.type;
	other.id = -1;
	other.size = { 0, 0 };
	other.type = GL_TEXTURE_2D;
	return *this;
}

void Texture::CreateAndLoad(const void* data, bool storage = false) {
	glGenTextures(1, &id);
	glBindTexture(type, id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	if (!storage)
		glTexImage2D(type, 0, GL_RGBA8UI, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexStorage2D(type, 1, GL_RGBA8UI, size.x, size.y);
}

void Texture::Load(glm::uvec2 Size, bool rectangle, bool storage = false) {
	type = (rectangle ? GL_TEXTURE_RECTANGLE : GL_TEXTURE_2D);
	size = Size;
	CreateAndLoad(nullptr);
}

void Texture::Load(const Image& image, bool rectangle) {
	type = (rectangle ? GL_TEXTURE_RECTANGLE : GL_TEXTURE_2D);
	size = image.GetSize();
	CreateAndLoad(image.GetRawData());
}

void Texture::Load(const std::string& name, bool rectangle) {
	Image tmp;
	tmp.Load(name);
	Load(tmp, rectangle);
}

Image Texture::Save() {
	Select();
	Image res;
	res.Load(size);
	glGetTexImage(type, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, res.GetRawData());
	return res;
}

void Texture::Reset() {
	size = { 0, 0 };
	type = GL_TEXTURE_2D;
	glDeleteTextures(1, &id);
}

void Texture::Select(uint32_t num) const {
	currentId = num == 0 ? id : currentId;
	glActiveTexture(GL_TEXTURE0 + num);
	glBindTexture(type, id);
}

void Texture::Update(const Image& image) {
	assert(currentId == id);
	assert(image.GetSize() == size);
	glTexImage2D(type, 0, GL_RGBA8UI, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetRawData());
	//glTexSubImage2D(type, 0, 0, 0, size.x, size.y, GL_RGBA, type, image.GetRawData());
}