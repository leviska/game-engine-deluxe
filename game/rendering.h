#pragma once
#include "batching.h"

#include <unordered_map>

class SpritePtr {
public:
	Sprite& operator*();
	const Sprite& operator*() const;

	Sprite* operator->();
	const Sprite* operator->() const;

	explicit operator bool() const;
private:
	size_t spriteId = std::numeric_limits<size_t>::max();
	size_t textureId = std::numeric_limits<size_t>::max();


	SpritePtr(size_t SpriteId, size_t TextureId) : spriteId(SpriteId), textureId(TextureId) {}

	friend Rendering;
};

class Rendering {
public:
	void Load();
	void Reset();

	SpritePtr Stage(const Sprite& sprite, size_t textureId);
	void Unstage(const SpritePtr& ptr);
private:
	std::unordered_map<uint32_t, BatchedRender> renders;
};
