#include "batching.h"

#include "resources.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void BatchedSprites::Load() {
	glGenBuffers(1, &textCoordsBuffer);
	glGenBuffers(1, &modelsBuffer);
}

void BatchedSprites::Reset() {
	sprites.clear();
	glDeleteBuffers(1, &textCoordsBuffer);
	glDeleteBuffers(1, &modelsBuffer);
}

void BatchedSprites::Update() {
	glBindVertexArray(Resources().GetBatchVAO());

	std::vector<glm::vec4> textCoords(sprites.size());
	std::vector<glm::mat4> models(sprites.size());
	for (size_t i = 0; i < sprites.size(); i++) {
		textCoords[i] = sprites[i].GetTextCoords();
		models[i] = sprites[i].GetModel();
	}

	glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sprites.size(), &textCoords[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, modelsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * sprites.size(), &models[0], GL_STATIC_DRAW);
}

void BatchedSprites::Update(size_t index) {
	glm::vec4 textCoords = sprites[index].GetTextCoords();
	glm::mat4 model = sprites[index].GetModel();
	glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(textCoords), sizeof(textCoords), &textCoords);
	glBindBuffer(GL_ARRAY_BUFFER, modelsBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(model), sizeof(model), &model);
}

void BatchedSprites::Draw() {
	if (sprites.empty())
		return;

	Resources().GetShader(Shaders::Batch).Select();
	uint32_t textureId = Resources().GetSpriteInfo(sprites.front().GetDataId()).TextureId;
	Resources().GetTexture(textureId).Select();
	glBindVertexArray(Resources().GetBatchVAO());
	glBindBuffer(GL_ARRAY_BUFFER, textCoordsBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, modelsBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, sprites.size());

	glBindVertexArray(0);
}