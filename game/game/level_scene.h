#pragma once

#include <game/scene.h>
#include <game/map.h>

#include <graphics/renderer.h>
#include <graphics/framebuffer.h>
#include <graphics/camera.h>

#include <entt/entt.hpp>

#include <string>

class LevelScene : public Scene {
public:
	void Load(const std::string& name);
	void Reset();

	void Update();
	void Clear();
	void Draw();
private:
	Renderer render;
	FrameBuffer gameScreen;
	Camera camera;

	entt::registry reg;
	MapView map;
};
