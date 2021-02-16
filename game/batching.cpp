#include "batching.h"

#include "resources.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void BatchedRender::Load(uint32_t TextureId, uint32_t ShaderId) {
	textureId = TextureId;
	shaderId = ShaderId;

	glGenBuffers(1, &dataBuffer);
}

void BatchedRender::Reset() {
	Clear();
	textureId = 0;
	shaderId = 0;
	glDeleteBuffers(1, &dataBuffer);
}

void BatchedRender::BindArray() {
	glBindVertexArray(Resources().GetBatchVAO());
}

void BatchedRender::Clear() {
	data.Clear();
}

size_t BatchedRender::Stage() {
	size_t id = data.Alloc();
	data[id] = Sprite();
	return id;
}

size_t BatchedRender::Stage(const Sprite& sprite) {
	size_t id = data.Alloc();
	data[id] = sprite;
	return id;
}

void BatchedRender::Unstage(size_t id) {
	data[id].Color = glm::vec4{ 0, 0, 0, 0 };
	data.Dealloc(id);
}

Sprite& BatchedRender::operator[](size_t id) {
	return data[id];
}

const Sprite& BatchedRender::operator[](size_t id) const {
	return data[id];
}

void BatchedRender::Draw() {
	if (data.Empty())
		return;

	Resources().GetShader(shaderId).Select();
	Resources().GetTexture(textureId).Select();
	BindArray();

	glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite) * data.size(), &data[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)8);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)16);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)24);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)32);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)48);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, data.size());

	glBindVertexArray(0);
}
