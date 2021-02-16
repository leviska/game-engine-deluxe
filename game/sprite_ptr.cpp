#include "sprite_ptr.h"

SpritePtr::SpritePtr(size_t SpriteId, BatchedRender* Render)
	: spriteId(SpriteId), render(Render) {}


Sprite& SpritePtr::operator*() {
	return (*render)[spriteId];
}

const Sprite& SpritePtr::operator*() const {
	return (*render)[spriteId];
}

Sprite* SpritePtr::operator->() {
	return &(*render)[spriteId];
}

const Sprite* SpritePtr::operator->() const {
	return &(*render)[spriteId];
}


SpritePtr::operator bool() const {
	return spriteId != std::numeric_limits<size_t>::max() && render != nullptr;
}

