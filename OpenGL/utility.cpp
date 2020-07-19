#include "utility.h"

#include <SDL.h>

FPSInfo FPS::Update() {
	FPSInfo res{ 0, 0 };
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	if (frames.empty()) {
		frames.push(end);
		return res;
	}
	res.dt = std::chrono::duration_cast<std::chrono::microseconds>(end - frames.back()).count();
	frames.push(end);
	while (!frames.empty()) {
		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - frames.front()).count();
		if (dur > 1000) {
			frames.pop();
		}
		else {
			break;
		}
	}
	if (frames.empty()) {
		res.fps = 0;
	}
	else {
		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - frames.front()).count();
		res.fps = static_cast<float>(frames.size() * 1000) / dur;
	}
	return res;
}