#pragma once

#include "scene.h"
#include "shapes.h"
#include "map.h"
#include "batching.h"
#include "framebuffer.h"
#include "camera.h"

#include "entt/entt.hpp"

class LevelEditorScene : public Scene {
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

	std::unique_ptr<SpriteOwner> preview;

	entt::registry db;
	MapView map;

	glm::ivec2 prevPos;
	std::string levelName;
	RectangleShape bb;
	uint32_t currentTile = 0;

	void DrawGui();
};
