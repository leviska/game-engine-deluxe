#pragma once

#include <cinttypes>
#include <string>
#include <entt/entt.hpp>

class Animation {
	uint32_t currentFrame = 0;
	uint32_t currentTime = 0;
public:
	uint32_t AnimId = 0;

	Animation() = default;
	Animation(const std::string& dataName);

	uint32_t GetFrame() { return currentFrame; }
	void SetFrame(uint32_t frame) { 
		currentFrame = frame;
		currentTime = 0;
	}
	uint32_t GetSpriteId();

	void Update(uint32_t dt);
};

void UpdateAnimations(entt::registry& db, uint32_t dt);
