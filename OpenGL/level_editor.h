#pragma once

#include "scene.h"
#include "shapes.h"
#include "map.h"
#include "batching.h"

#include "entt/entt.hpp"

class LevelEditorScene : public Scene {
	std::unordered_map<uint32_t, BatchedRender> renders;

	entt::registry db;
	WallMap map;

	std::string levelName;

	RectangleShape bb;

	struct GUIData {
		std::string CurrentTile = "Wall00";
	};

	GUIData data;

	void DrawGui();
public:
	void Load(const std::string& name);
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;
};
