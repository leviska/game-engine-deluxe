#include "animation.h"

/*
Animation::Animation(const std::string& dataName) {
	AnimId = Resources().GetAnimationInfoId(dataName);
}

void Animation::Update(uint32_t dt) {
	currentTime += dt;
	const auto& info = Resources().GetAnimationInfo(AnimId);
	while (currentTime >= info[currentFrame].Delay) {
		currentTime -= info[currentFrame].Delay;
		currentFrame++;
		if (currentFrame >= info.size()) {
			currentFrame = 0;
		}
	}
}

uint32_t Animation::GetSpriteId() {
	const auto& info = Resources().GetAnimationInfo(AnimId);
	return info[currentFrame].SpriteId;
}

void UpdateAnimations(entt::registry& db, uint32_t dt) {
	auto rendView = db.view<Animation>();
	for (auto id : rendView) {
		rendView.get<Animation>(id).Update(dt);
	}
}
*/