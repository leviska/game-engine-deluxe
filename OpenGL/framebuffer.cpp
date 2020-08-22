#include "framebuffer.h"

#include "resources.h"
#include "game.h"

#include <stdexcept>

#include <glad/glad.h>

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept {
	id = other.id;
	other.id = 0;
	texture = std::move(other.texture);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
	id = other.id;
	other.id = 0;
	texture = std::move(other.texture);
	return *this;
}

void FrameBuffer::Load(glm::uvec2 size, bool rectangle) {
	texture.Load(size, rectangle);

	glGenFramebuffers(1, &id);
	Select();
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture.GetType(), texture.GetId(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Framebuffer isn't complete");
	}
	SelectWindow();
}

void FrameBuffer::Reset() {
	glDeleteFramebuffers(1, &id);
}

void FrameBuffer::Clear(glm::vec4 color) {
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::Select() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	Game().UpdateViewport(texture.GetSize());
}

void FrameBuffer::SelectWindow() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Game().GetWindow().UpdateViewport();
}

void FrameBuffer::Draw(glm::vec2 pos, float scale, Shader& shader) {
	shader.Select();
	texture.Select();
	glBindVertexArray(Resources().GetSpriteVAO());

	shader.SetVec2("Position", pos);
	shader.SetVec3("Transform", glm::vec3(texture.GetSize(), scale));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
