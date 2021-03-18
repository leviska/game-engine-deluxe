#pragma once

#include <game/scene.h>
#include <game/map.h>

#include <engine/state_machine.h>

#include <graphics/particles.h>
#include <graphics/framebuffer.h>
#include <graphics/camera.h>

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
