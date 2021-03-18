#pragma once

#include <game/scene.h>
#include <game/map.h>

#include <graphics/shapes.h>
#include <graphics/framebuffer.h>
#include <graphics/camera.h>

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>

class EditorScene : public Scene {
public:
	void Load(const std::string& name = "sandboxLevel");
	void Reset() override;

	void Update() override;
	void Clear() override;
	void Draw() override;

	const std::string& CurrentLevel() const;
private:
	entt::registry reg;
	Renderer render;
	MapView map;

	FrameBuffer frameBuffer;
	Camera camera;

	std::string levelName;

	entt::entity currentId{ entt::null };
	RectangleShape bb;
	RectangleShape curbb;

	nlohmann::json entityPresets;
	nlohmann::json curPreset;
	std::string curPresetName;
	std::string presetsName;

	void ShowEntity(entt::entity id);

	void DrawGui();
	void DrawMainGui();
	void DrawEntityGui();
	void DrawSelectorGui();
	void DrawEntityPresetsGui();

	void ResetMap();
	void LoadMap();

	void UpdateSystems();
	void UpdateMouseEditor();
	void UpdateMoving();
	void UpdateBB();
};
