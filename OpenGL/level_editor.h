#pragma once

#include "scene.h"
#include "shapes.h"

class LevelEditorScene : public Scene {
	BatchedRender tileRender;

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
	void Load(std::string name);
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;
};
