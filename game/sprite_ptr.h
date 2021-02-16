#pragma once

#include "batching.h"

class Rendering;

class SpritePtr {
public:
	Sprite& operator*();
	const Sprite& operator*() const;

	Sprite* operator->();
	const Sprite* operator->() const;

	explicit operator bool() const;
private:
	size_t spriteId = std::numeric_limits<size_t>::max();
	BatchedRender* render = nullptr;

	SpritePtr(size_t SpriteId, BatchedRender* Render);

	friend Rendering;
};
