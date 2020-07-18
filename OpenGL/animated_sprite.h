#pragma once
#include "sprite.h"

class AnimatedSprite : public Sprite {
	uint32_t currentFrame = 0;
	uint32_t currentTime = 0;
public:
	uint32_t AnimDataId = 0;

	void Load(uint32_t dataId) override;
	void Reset();

	void Update(uint32_t dt);
};
