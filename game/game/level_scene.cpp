#include <game/level_scene.h>
#include <game/game.h>

#include <systems/level.h>
#include <systems/moving.h>
#include <systems/obstructing.h>

#include <resources/consts.h>
#include <resources/shaders.h>

void LevelScene::Load(const std::string& name) {
	LoadLevel(name, reg, render, map);
	render.Load();

	camera = reg.create();
	reg.emplace<Camera>(camera, glm::vec2{ 0, 0 }, glm::vec2{ Consts().CanvasSize / 2u });

	gameScreen.Load(Consts().CanvasSize);
}

void LevelScene::Reset() {
	map.clear();
	reg.clear();
	render.Reset();
	gameScreen.Reset();
}

void LevelScene::Update() {
	UpdateSystems();
}

void LevelScene::UpdateSystems() {
	UpdateMoving(reg);
	CheckForObstructing(reg, map);
	MoveEntities(reg, map);
	UpdateFollowCamera(reg);
	UpdateRenderPositions(reg);
	assert(MapValid(reg, map));
}

void LevelScene::Clear() {
	gameScreen.Select();
	gameScreen.Clear(RGBA({ 0, 0, 0, 255 }));
	FrameBuffer::SelectWindow();
}

void LevelScene::Draw() {
	// --------- gameScreen --------- 
	gameScreen.Select();
	render.Draw(reg.get<Camera>(camera));

	// --------- screen --------- 
	DrawFramebufferToScreen(gameScreen);
}
