#include <fps.h>

#include <utility.h>

#include <imgui.h>

#include <iostream>
#include <thread>

void FPS::Draw() const {
	ImGui::Begin("Debug info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("FPS: %d", static_cast<int>(lastFrame.fps));
	uint32_t dt = lastFrame.dt;
	uint32_t cnt = 1;
	if (!avgDts.empty()) {
		dt = avgDts.front().dt;
	}
	ImGui::Text("Max dt: %.2f", static_cast<float>(dt) / 1000.0f);
	ImGui::Text("Avg dt: %.2f", avgLast);
	ImGui::End();
}

void FPS::UpdateDt(uint32_t dt, const TimePoint& current) {
	while (!avgDts.empty() && avgDts.back().dt < dt) {
		avgDts.pop_back();
	}
	avgSum += dt;
	avgDts.push_back({ current, dt });
	while (std::chrono::duration_cast<Microseconds>(current - avgDts.front().index).count() >= 1000000) {
		avgDts.pop_front();
	}
	const int32_t updatePeriod = 10;
	if (!avgFrame) {
		avgFrame = updatePeriod;
		avgLast = static_cast<float>(avgSum) / (1000.0f * updatePeriod);
		avgSum = 0;
	}
	avgFrame--;
}

void FPS::UpdateFrames(const TimePoint& current) {
	frames.push(current);
	while (!frames.empty()) {
		auto dur = std::chrono::duration_cast<Microseconds>(current - frames.front()).count();
		if (dur > 1000000) {
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
	res.dt = to_ui32(std::min(60*1000ll, std::chrono::duration_cast<Microseconds>(end - frames.back()).count()));
	
	UpdateFrames(end);
	
	auto dur = std::chrono::duration_cast<Milliseconds>(frames.back() - frames.front()).count();
	res.fps = static_cast<float>(frames.size() * 1000) / dur;
	
	lastFrame = res;
	return res;
}

void FPS::LimitFPS(uint32_t fps) {
	TimePoint end = std::chrono::steady_clock::now();
	uint32_t dt = to_ui32(std::chrono::duration_cast<Microseconds>(end - lastTime).count());
	if (fps > 0) {
		uint32_t time = (1000000u / (fps + 1));
		if (time > dt) {
			std::this_thread::sleep_for(Microseconds(time - dt));
		}
	}
	UpdateDt(dt, end);
	lastTime = std::chrono::steady_clock::now();
}