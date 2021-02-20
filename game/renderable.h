#pragma once

#include "sprite_ptr.h"

#include <vector>

class Renderable {
public:
	Renderable() = default;
	Renderable(Renderable&&) = default;
	~Renderable();

	Renderable(const Renderable&) = delete;

	void Add(SpritePtr ptr);
	SpritePtr Get(size_t id = 0) const;
	void Pop();
private:
	std::vector<SpritePtr> sprites;
};
