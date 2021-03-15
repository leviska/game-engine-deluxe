#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"
#include "input.h"

#include "consts.h"
#include "shaders.h"

#include "imgui.h"

#include <fstream>
#include <queue>
#include <thread>

void SandboxScene::Load() {
	//LoadMap(map, db, render, std::string("sandboxLevel"));
	render.Load();

	glm::vec2 halfCanvas(Consts().CanvasSize / 2u);
	camera = Camera{ halfCanvas, halfCanvas };

	pixelArt.Load(Consts().CanvasSize);
}

void SandboxScene::Reset() {
	map.clear();
	db.clear();
	pixelArt.Reset();
	render.Reset();
}

void SandboxScene::Update() {
	camera.UpdateFreeCamera();

	frame++;
	time += Game().FPS().dt / 1000000.f;
}

void SandboxScene::Clear() {
	pixelArt.Select();
	//pixelArt.Clear(RGBA({ 31, 14, 28, 255 }));
	pixelArt.Clear(RGBA({ 31 / 2, 14 / 2, 28 / 2, 255 }));
	FrameBuffer::SelectWindow();
}

void SandboxScene::Draw() {
	// --------- pixelArt --------- 
	pixelArt.Select();
	render.Draw(camera);

	// --------- screen --------- 
	DrawFramebufferToScreen(pixelArt);
}
