#include "sprite_ptr.h"

SpritePtr::SpritePtr(size_t SpriteId, BatchedRender* Render)
	: spriteId(SpriteId), render(Render) {}


Sprite& SpritePtr::operator*() {
	assert((*this));
	return (*render)[spriteId];
}

const Sprite& SpritePtr::operator*() const {
	assert((*this));
	return (*render)[spriteId];
}

Sprite* SpritePtr::operator->() {
	assert((*this));
	return &(*render)[spriteId];
}

const Sprite* SpritePtr::operator->() const {
	assert((*this));
	return &(*render)[spriteId];
}


SpritePtr::operator bool() const {
	return spriteId != std::numeric_limits<size_t>::max() && render != nullptr;
}

void SpritePtr::Unstage() {
	assert((*this));
	render->Unstage(*this);
}



SpriteOwner::SpriteOwner(SpritePtr ptr) : sprite(ptr) {}

SpriteOwner::~SpriteOwner() {
	sprite.Unstage();
}


Sprite& SpriteOwner::operator*() {
	return *sprite;
}

const Sprite& SpriteOwner::operator*() const {
	return *sprite;
}


Sprite* SpriteOwner::operator->() {
	return &*sprite;
}

const Sprite* SpriteOwner::operator->() const {
	return &*sprite;
}


SpritePtr SpriteOwner::Get() const {
	return sprite;
}
