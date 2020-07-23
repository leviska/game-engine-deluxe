#pragma once
#include <chrono>
#include <queue>

struct FPSInfo {
	float fps;
	uint32_t dt;
};


class FPS {
	using TimePoint = std::chrono::steady_clock::time_point;
	using Milliseconds = std::chrono::milliseconds;
	using Microseconds = std::chrono::microseconds;

	std::queue<TimePoint> frames;
	TimePoint lastTime;

	struct DTInfo {
		TimePoint index;
		uint32_t dt;
	};

	std::deque<DTInfo> dts;
	uint64_t sum = 0;

	FPSInfo lastFrame;

	void UpdateDt(uint32_t dt, const TimePoint& current);
	void UpdateFrames(const TimePoint& current);
public:
	void Draw() const;
	FPSInfo Update();
	void LimitFPS(uint32_t fps);

	const FPSInfo& LastFrame() const { return lastFrame; }
};