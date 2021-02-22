#pragma once

#include "batching.h"

class SpritePtr {
public:
	SpritePtr() = default;

	Sprite& operator*();
	const Sprite& operator*() const;

	Sprite* operator->();
	const Sprite* operator->() const;

	explicit operator bool() const;

	void Unstage();
private:
	size_t spriteId = std::numeric_limits<size_t>::max();
	BatchedRender* render = nullptr;

	SpritePtr(size_t id, BatchedRender* batchRender);
	
	friend BatchedRender;
};

class SpriteOwner {
public:
	SpriteOwner(SpritePtr ptr);
	SpriteOwner(SpriteOwner&& other) noexcept;
	SpriteOwner& operator=(SpriteOwner&&) noexcept;
	SpriteOwner(const SpriteOwner&) = delete;
	SpriteOwner& operator=(const SpriteOwner&) = delete;
	~SpriteOwner();

	Sprite& operator*();
	const Sprite& operator*() const;

	Sprite* operator->();
	const Sprite* operator->() const;

	SpritePtr Get() const;
private:
	SpritePtr sprite;
};