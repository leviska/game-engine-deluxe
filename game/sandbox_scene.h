#pragma once

#include "scene.h"

#include "particles.h"
#include "state_machine.h"
#include "map.h"
#include "framebuffer.h"
#include "camera.h"

#include <entt/entt.hpp>

class SandboxScene : public Scene {
public:
	FrameBuffer pixelArt;
	Renderer render;
	Camera camera;

	float time = 0;
	int frame = 0;

	entt::registry db;
	MapView map;

	void Load();
	void Reset() override;

	void Lights();
	
	void Update() override;
	void Clear() override;
	void Draw() override;
};
