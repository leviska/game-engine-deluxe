#pragma once

#include "scene.h"

#include "batching.h"
#include "particles.h"
#include "state_machine.h"
#include "map.h"

#include "entt/entt.hpp"

class SandboxScene : public Scene {
public:
	std::unordered_map<uint32_t, BatchedRender> renders;

	entt::registry db;
	WallMap map;

	void Load();
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;
};