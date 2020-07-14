#include "batching.h"

#include "resources.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void BatchedSprites::Load() {
	glGenBuffers(1, &positionsBuffer);
	glGenBuffers(1, &textCoordsBuffer);
	glGenBuffers(1, &colorsBuffer);
	glGenBuffers(1, &transformsBuffer);
}

void BatchedSprites::Reset() {
	sprites.clear();
	glDeleteBuffers(1, &positionsBuffer);
	glDeleteBuffers(1, &textCoordsBuffer);
	glDeleteBuffers(1, &colorsBuffer);
	glDeleteBuffers(1, &transformsBuffer);
}

void BatchedSprites::Update() {
	glBindVertexArray(Resources().GetBatchVAO());

	positions.resize(sprites.size());
	textCoords.resize(sprites.size());
	colors.resize(sprites.size());
	transforms.resize(sprites.size());

	for (size_t i = 0; i < sprites.size(); i++) {
		positions[i] = sprites[i].GetPos();
	}
	for (size_t i = 0; i < sprites.size(); i++) {
		textCoords[i] = sprites[i].GetTextCoords();
	}
	
	for (size_t i = 0; i < sprites.size(); i++) {
		colors[i] = sprites[i].GetColor();
	}
	for (size_t i = 0; i < sprites.size(); i++) {
		transforms[i] = sprites[i].GetTransform();
	}

	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * sprites.size(), &positions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sprites.size(), &textCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sprites.size(), &colors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, transformsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sprites.size(), &transforms[0], GL_STATIC_DRAW);
}

void BatchedSprites::Draw() {
	if (sprites.empty())
		return;

	Resources().GetShader(Shaders::Batch).Select();
	uint32_t textureId = Resources().GetSpriteInfo(sprites.front().GetDataId()).TextureId;
	Resources().GetTexture(textureId).Select();
	glBindVertexArray(Resources().GetBatchVAO());
	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, transformsBuffer);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, sprites.size());

	glBindVertexArray(0);
}