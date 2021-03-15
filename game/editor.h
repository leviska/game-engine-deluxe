#pragma once

#include "scene.h"
#include "shapes.h"
#include "map.h"
#include "framebuffer.h"
#include "camera.h"

#include "entt/entt.hpp"

class EditorScene : public Scene {
public:
	void Load(const std::string& name = "sandboxLevel");
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;

	const std::string& CurrentLevel() const;
private:
	Renderer render;
	Camera camera;
	FrameBuffer frameBuffer;

	entt::registry reg;
	MapView map;

	std::string levelName;
	entt::entity currentId{ entt::null };
	RectangleShape bb;

	glm::ivec2 selectorPos{ 0, 0 };

	void DrawGui();

	void DrawMainGui();
	void DrawEntityGui();
	void DrawSelectorGui();
};
