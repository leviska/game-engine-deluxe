#include "utility.h"


float FPS::Update() {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
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
		return 0;
	}
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - frames.front()).count();
	return static_cast<float>(frames.size() * 1000) / dur;
}