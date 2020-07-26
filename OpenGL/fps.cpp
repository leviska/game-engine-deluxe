#include "fps.h"

#include "imgui.h"

#include <iostream>
#include <SDL.h>

void FPS::Draw() const {
	ImGui::Begin("Debug info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %d", static_cast<int>(lastFrame.fps));
	uint32_t dt = lastFrame.dt;
	uint32_t cnt = 1;
	if (!avgDts.empty()) {
		dt = avgDts.front().dt;
	}
	ImGui::Text("Max dt: %.1f", static_cast<float>(dt) / 1000.0f);
	ImGui::Text("Avg dt: %.1f", avgLast);
	ImGui::End();
}

void FPS::UpdateDt(uint32_t dt, const TimePoint& current) {
	while (!avgDts.empty() && avgDts.back().dt < dt) {
		avgDts.pop_back();
	}
	avgSum += dt;
	avgDts.push_back({ current, dt });
	while (std::chrono::duration_cast<Milliseconds>(current - avgDts.front().index).count() >= 1000) {
		avgDts.pop_front();
	}
	if (!avgFrame) {
		avgFrame = kAvgFrameRate;
		avgLast = static_cast<float>(avgSum) / (1000.0f * kAvgFrameRate);
		avgSum = 0;
	}
	avgFrame--;
}

void FPS::UpdateFrames(const TimePoint& current) {
	frames.push(current);
	while (!frames.empty()) {
		auto dur = std::chrono::duration_cast<Milliseconds>(current - frames.front()).count();
		if (dur > 1000) {
			frames.pop();
		}
		else {
			break;
		}
	}
}

FPSInfo FPS::Update() {
	FPSInfo res{ 0, 0 };
	TimePoint end = std::chrono::steady_clock::now();
	if (frames.empty()) {
		frames.push(end);
		lastFrame = res;
		return res;
	}
	res.dt = static_cast<uint32_t>(std::min(60*1000ll, std::chrono::duration_cast<Microseconds>(end - frames.back()).count()));
	
	UpdateFrames(end);
	
	auto dur = std::chrono::duration_cast<Milliseconds>(frames.back() - frames.front()).count();
	res.fps = static_cast<float>(frames.size() * 1000) / dur;
	
	lastFrame = res;
	return res;
}

void FPS::LimitFPS(uint32_t fps) {
	TimePoint end = std::chrono::steady_clock::now();
	uint32_t dt = static_cast<uint32_t>(std::chrono::duration_cast<Microseconds>(end - lastTime).count());
	uint32_t time = (1000000u / fps);
	if (time > dt) {
		SDL_Delay((time - dt) / 1000);
	}
	UpdateDt(dt, end);
	lastTime = std::chrono::steady_clock::now();
}