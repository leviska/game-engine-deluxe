#pragma once

#include "batching.h"
#include "particles.h"
#include "state_machine.h"
#include "map.h"

#include "entt/entt.hpp"

class Scene {
public:
	virtual void Reset() = 0;

	virtual void Update() = 0;
	virtual void Clear() = 0;
	virtual void Draw() = 0;
};

class SandboxScene : public Scene {
public:
	BatchedRender tileRender;

	entt::registry db;
	WallMap map;

	void Load();
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;
};

class LevelScene : public Scene {};
