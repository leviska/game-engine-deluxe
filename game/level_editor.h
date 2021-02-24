#pragma once

#include "scene.h"
#include "shapes.h"
#include "map.h"
#include "batching.h"
#include "framebuffer.h"

#include "entt/entt.hpp"

class LevelEditorScene : public Scene {
	Renderer render;
	FrameBuffer frameBuffer;

	entt::registry db;
	MapView map;

	glm::ivec2 prevPos;

	std::string levelName;

	RectangleShape bb;

	struct GUIData {
		std::string CurrentTile = "Wall00";
	};

	GUIData data;

	void DrawGui();
public:
	void Load(const std::string& name = "sandboxLevel");
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;
};
