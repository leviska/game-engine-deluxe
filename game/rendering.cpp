#include "rendering.h"


Sprite& SpritePtr::operator*() {
	
}

const Sprite& SpritePtr::operator*() const {

}


Sprite* SpritePtr::operator->() {

}

const Sprite* SpritePtr::operator->() const {

}


explicit SpritePtr::operator bool() const {
	return spriteId == std::numeric_limits<size_t>::max();
}
