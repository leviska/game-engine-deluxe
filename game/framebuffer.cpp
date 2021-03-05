#include "framebuffer.h"

#include "glbuffers.h"
#include "game.h"
#include "image.h"
#include "assertion.h"

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
		THROWERROR("Framebuffer isn't complete");
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
	Game().UpdateViewport(Game().GetWindow().GetSize());
}

void FrameBuffer::Draw(glm::vec2 pos, float scale, const Shader& shader) const {
	shader.Select();
	texture.Select();
	Buffers().SpriteBuffer.Bind();

	shader.SetVec2("Position", pos);
	shader.SetVec3("Transform", glm::vec3(texture.GetSize(), scale));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	ObjectBuffer::Unbind();
}
