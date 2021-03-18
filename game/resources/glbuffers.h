#pragma once

#include <texture.h>
#include <sprite.h>
#include <singleton.h>
#include <named_vector.h>

#include <cinttypes>

struct ObjectBuffer {
	uint32_t VAO = 0;
	uint32_t VBO = 0;
	uint32_t EBO = 0;

	void Bind() const;

	static void Unbind();
};

class BuffersImpl : public Singleton {
public:
	ObjectBuffer SpriteBuffer;
	ObjectBuffer BatchBuffer;
	ObjectBuffer ParticleBuffer;
	ObjectBuffer EmptyBuffer;
	
	void Load();
	void Reset();
private:
	~BuffersImpl() { Reset(); }

	void LoadSpriteVAO();
	void LoadBatchVAO();
	void LoadParticleVAO();
	void LoadShapeVAO();

	friend BuffersImpl& BuffersMut();
};

const BuffersImpl& Buffers();
BuffersImpl& BuffersMut();
