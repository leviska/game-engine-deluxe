#include <sprite_ptr.h>

#include <renderer.h>

SpritePtr::SpritePtr(size_t SpriteId, Renderer* Render)
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
	return render != nullptr && spriteId != std::numeric_limits<size_t>::max();
}

void SpritePtr::Unstage() {
	assert((*this));
	render->Unstage(spriteId);
}



SpriteOwner::SpriteOwner(SpritePtr ptr) : sprite(ptr) {}


SpriteOwner::SpriteOwner(SpriteOwner&& other) noexcept {
	sprite = other.sprite;
	other.sprite = SpritePtr();
}

SpriteOwner& SpriteOwner::operator=(SpriteOwner&& other) noexcept {
	sprite = other.sprite;
	other.sprite = SpritePtr();
	return *this;
}

 
SpriteOwner::~SpriteOwner() {
	if (sprite) {
		sprite.Unstage();
	}
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
