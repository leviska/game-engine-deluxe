#pragma once
#include <chrono>
#include <queue>

class FPS {
	std::queue<std::chrono::steady_clock::time_point> frames;
public:
	float Update();
};