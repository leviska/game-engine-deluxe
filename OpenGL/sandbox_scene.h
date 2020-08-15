#pragma once

#include "scene.h"

#include "batching.h"
#include "particles.h"
#include "state_machine.h"
#include "map.h"
#include "framebuffer.h"

#include "entt/entt.hpp"

class SandboxScene : public Scene {
public:
	FrameBuffer frameBuffer;
	FrameBuffer obstructBuffer;
	FrameBuffer tmpBuffer;

	std::unordered_map<uint32_t, BatchedRender> renders;
	BatchedRender obstructRender;

	entt::registry db;
	WallMap map;

	void Load();
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;
};