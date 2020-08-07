#include "animated_sprite.h"

#include "resources.h"

void AnimatedSprite::Load(std::string dataName) {
	Load(Resources().GetAnimationInfoId(dataName));
}

void AnimatedSprite::Load(uint32_t dataId) {
	AnimDataId = dataId;
	Visible = true;
}

void AnimatedSprite::Reset() {
	Sprite::Reset();
	AnimDataId = 0;
	currentFrame = 0;
	currentTime = 0;
}


void AnimatedSprite::Update(uint32_t dt) {
	currentTime += dt;
	const auto& info = Resources().GetAnimationInfo(AnimDataId);
	while (currentTime >= info[currentFrame].Delay) {
		currentTime -= info[currentFrame].Delay;
		currentFrame++;
		if (currentFrame >= info.size()) {
			currentFrame = 0;
		}
	}
	DataId = info[currentFrame].SpriteId;
}
