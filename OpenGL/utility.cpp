#include "utility.h"

#include "imgui.h"

#include <iostream>
#include <SDL.h>

void FPS::Draw() const {
	ImGui::Begin("Debug info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %d", static_cast<int>(lastFrame.fps));
	uint32_t dt = lastFrame.dt;
	uint32_t cnt = 1;
	if (!dts.empty()) {
		dt = dts.front().dt;
		cnt = dts.size();
	}
	ImGui::Text("Max dt: %.1f", static_cast<float>(dt) / 1000.0f);
	ImGui::Text("Avg dt: %.1f", static_cast<float>(sum) / (1000.0f * cnt));
	ImGui::End();
}

void FPS::UpdateDt(uint32_t dt, const TimePoint& current) {
	while (!dts.empty() && dts.back().dt < dt) {
		sum -= dts.back().dt;
		dts.pop_back();
	}
	sum += dt;
	dts.push_back({ current, dt });
	while (std::chrono::duration_cast<Milliseconds>(current - dts.front().index).count() >= 1000) {
		sum -= dts.front().dt;
		dts.pop_front();
	}
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
	res.dt = std::chrono::duration_cast<Microseconds>(end - frames.back()).count();
	
	UpdateFrames(end);
	
	auto dur = std::chrono::duration_cast<Milliseconds>(frames.back() - frames.front()).count();
	res.fps = static_cast<float>(frames.size() * 1000) / dur;
	
	lastFrame = res;
	return res;
}

void FPS::LimitFPS(uint32_t fps) {
	TimePoint end = std::chrono::steady_clock::now();
	uint32_t dt = std::chrono::duration_cast<Microseconds>(end - lastTime).count();
	uint32_t time = (1000000u / fps);
	if (time > dt) {
		SDL_Delay((time - dt) / 1000);
	}
	UpdateDt(dt, end);
	lastTime = std::chrono::steady_clock::now();
}