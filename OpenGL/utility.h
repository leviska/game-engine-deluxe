#pragma once
#include <chrono>
#include <queue>

struct FPSInfo {
	float fps;
	uint32_t dt;
};

class FPS {
	std::queue<std::chrono::steady_clock::time_point> frames;
public:
	FPSInfo Update();
};