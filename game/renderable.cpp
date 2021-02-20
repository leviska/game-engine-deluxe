#include "renderable.h"

Renderable::~Renderable() {
	while (!sprites.empty()) {
		Pop();
	}
}


void Renderable::Add(SpritePtr ptr) {
	sprites.push_back(ptr);
}

SpritePtr Renderable::Get(size_t id = 0) const {
	return sprites[id];
}

void Renderable::Pop() {
	sprites.back().Unstage();
	sprites.pop_back();
}
