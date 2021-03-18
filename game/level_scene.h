#pragma once

#include <scene.h>
#include <renderer.h>
#include <framebuffer.h>
#include <map.h>
#include <camera.h>

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
