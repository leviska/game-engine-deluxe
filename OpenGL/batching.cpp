#include "batching.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void BatchedRender::Load(uint32_t TextureId, uint32_t ShaderId) {
	textureId = TextureId;
	shaderId = ShaderId;

	glGenBuffers(1, &positionsBuffer);
	glGenBuffers(1, &textCoordsBuffer);
	glGenBuffers(1, &colorsBuffer);
	glGenBuffers(1, &transformsBuffer);
}

void BatchedRender::Reset() {
	Clear();
	textureId = 0;
	glDeleteBuffers(1, &positionsBuffer);
	glDeleteBuffers(1, &textCoordsBuffer);
	glDeleteBuffers(1, &colorsBuffer);
	glDeleteBuffers(1, &transformsBuffer);
}

void BatchedRender::BindArray() {
	glBindVertexArray(Resources().GetBatchVAO());
}

void BatchedRender::Clear() {
	positions.clear();
	textCoords.clear();
	colors.clear();
	transforms.clear();
}

void BatchedRender::Draw() {
	if (positions.empty())
		return;

	Resources().GetShader(shaderId).Select();
	Resources().GetTexture(textureId).Select();
	BindArray();

	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * textCoords.size(), &textCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, colorsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colors.size(), &colors[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, transformsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * transforms.size(), &transforms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, positions.size());

	glBindVertexArray(0);
}
