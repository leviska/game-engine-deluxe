#include <level_scene.h>

#include <game.h>

#include <resources/consts.h>
#include <resources/shaders.h>

void LevelScene::Load(const std::string& name) {
	//LoadMap(map, reg, render, name);
	render.Load();

	glm::vec2 halfCanvas(Consts().CanvasSize / 2u);
	camera = Camera{ halfCanvas, halfCanvas };

	gameScreen.Load(Consts().CanvasSize);
}

void LevelScene::Reset() {
	map.clear();
	reg.clear();
	render.Reset();
	gameScreen.Reset();
}

void LevelScene::Update() {
	camera.UpdateFreeCamera();
}

void LevelScene::Clear() {
	gameScreen.Select();
	gameScreen.Clear(RGBA({ 31 / 2, 14 / 2, 28 / 2, 255 }));
	FrameBuffer::SelectWindow();
}

void LevelScene::Draw() {
	// --------- gameScreen --------- 
	gameScreen.Select();
	render.Draw(camera);

	// --------- screen --------- 
	DrawFramebufferToScreen(gameScreen);
}
