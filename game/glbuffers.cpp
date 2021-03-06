#include "glbuffers.h"

#include "assertion.h"

void ObjectBuffer::Bind() const {
	glBindVertexArray(VAO);
}

void ObjectBuffer::Unbind() {
	glBindVertexArray(0);
}


void DeleteObjectBuffer(ObjectBuffer& buffer) {
	glDeleteVertexArrays(1, &buffer.VAO);
	glDeleteBuffers(1, &buffer.VBO);
	glDeleteBuffers(1, &buffer.EBO);

	buffer.VAO = 0;
	buffer.VBO = 0;
	buffer.EBO = 0;
}


void BuffersImpl::Load() {
	DOUT() << "Loading GL Buffers" << std::endl;

	LoadSpriteVAO();
	LoadBatchVAO();
	LoadParticleVAO();
	LoadShapeVAO();

	DOUT() << "Successfully loaded GL Buffers" << std::endl;
}

void BuffersImpl::Reset() {
	DeleteObjectBuffer(SpriteBuffer);
	DeleteObjectBuffer(BatchBuffer);
	DeleteObjectBuffer(ParticleBuffer);
	DeleteObjectBuffer(EmptyBuffer);
}


void CreateVAO(ObjectBuffer& buffer) {
	glGenVertexArrays(1, &buffer.VAO);
}

void CreateVEBO(ObjectBuffer& buffer) {
	glGenBuffers(1, &buffer.VBO);
	glGenBuffers(1, &buffer.EBO);
}

void BindRectangle(ObjectBuffer& buffer) {
	constexpr float RectVertices[] = {
		0.5f, 0.5f,  // top right
		0.5f, -0.5f,  // bottom right
		-0.5f, -0.5f,  // bottom left
		-0.5f, 0.5f   // top left 
	};
	constexpr uint32_t RectIndices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(RectVertices), RectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RectIndices), RectIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void BuffersImpl::LoadSpriteVAO() {
	CreateVAO(SpriteBuffer);
	SpriteBuffer.Bind();
	CreateVEBO(SpriteBuffer);

	BindRectangle(SpriteBuffer);

	ObjectBuffer::Unbind();
}

void BuffersImpl::LoadBatchVAO() {
	CreateVAO(BatchBuffer);
	BatchBuffer.Bind();
	CreateVEBO(BatchBuffer);

	BindRectangle(BatchBuffer);

	// struct fields
	for (int i = 1; i < 8; i++) {
		glEnableVertexAttribArray(i);
		glVertexAttribDivisor(i, 1);
	}

	ObjectBuffer::Unbind();
}

void BuffersImpl::LoadParticleVAO() {
	CreateVAO(ParticleBuffer);
	ParticleBuffer.Bind();

	glEnableVertexAttribArray(0);
	glVertexAttribDivisor(0, 1);

	ObjectBuffer::Unbind();
}

void BuffersImpl::LoadShapeVAO() {
	CreateVAO(EmptyBuffer);
	// EmptyBuffer.Bind();

	// ObjectBuffer::Unbind();
}


const BuffersImpl& Buffers() {
	return BuffersMut();
}

BuffersImpl& BuffersMut() {
	static BuffersImpl impl;
	return impl;
}
