#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"
#include "resources.h"
#include "input.h"

#include "imgui.h"

#include <fstream>
#include <queue>

void SandboxScene::Load() {
	LoadMap(map, db, render, std::string("sandboxLevel"));
	
	pixelArt.Load(Resources().CanvasSize);
}

void SandboxScene::Reset() {
	map.clear();
	db.clear();
	pixelArt.Reset();
	render.Reset();
}

void SandboxScene::Update() {
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
	render.Draw();

	// --------- screen --------- 
	FrameBuffer::SelectWindow();
	pixelArt.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), Resources().GetShader("BufferShader"));
}
