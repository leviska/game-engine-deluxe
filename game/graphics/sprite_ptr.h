#pragma once

#include <graphics/sprite.h>

class Renderer;

class SpritePtr {
public:
	SpritePtr() = default;
	SpritePtr(size_t id, Renderer* rend);

	Sprite& operator*();
	const Sprite& operator*() const;

	Sprite* operator->();
	const Sprite* operator->() const;

	explicit operator bool() const;

	void Unstage();
private:
	size_t spriteId = std::numeric_limits<size_t>::max();
	Renderer* render;
	
	friend Renderer;
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