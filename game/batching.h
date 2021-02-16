#pragma once
#include "sprite.h"
#include "vector_allocator.h"
#include <vector>

class BatchedRender {
public:
	void Load(uint32_t TextureId, uint32_t ShaderId = static_cast<uint32_t>(Shaders::Batch));
	void Reset();
	
	void Clear();
	
	size_t Stage();
	size_t Stage(const Sprite& sprite);
	void Unstage(size_t id);

	Sprite& operator[](size_t id);
	const Sprite& operator[](size_t id) const;

	void Draw();
private:
	uint32_t textureId = 0;
	uint32_t shaderId = 0;

	uint32_t dataBuffer = 0;
	VectorAlloc<Sprite> data;

	void BindArray();
	
};
