#include "batching.h"

#include "resources.h"
#include "sprite_ptr.h"

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

SpritePtr BatchedRender::Stage() {
	size_t id = data.Alloc();
	data[id] = Sprite();
	return SpritePtr(id, this);
}

SpritePtr BatchedRender::Stage(const Sprite& sprite) {
	size_t id = data.Alloc();
	data[id] = sprite;
	return SpritePtr(id, this);
}

void BatchedRender::Unstage(SpritePtr ptr) {
	ptr->Color = glm::vec4{ 0, 0, 0, 0 };
	data.Dealloc(ptr.spriteId);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite) * data.Size(), &data[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)0);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)8);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)12);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)20);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)28);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)36);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Sprite), (void*)52);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, data.Size());

	glBindVertexArray(0);
}
