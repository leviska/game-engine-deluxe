#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"

#include <fstream>

void SandboxScene::Load() {
	renders[0].Load(0);
	map.Load(db);
	LoadMap(map, "sandboxLevel");
}

void SandboxScene::Reset() {
	db.clear();
	ResetRenders(renders);
}

void SandboxScene::Update() {}

void SandboxScene::Clear() {
	ClearRenders(renders);
}

void SandboxScene::Draw() {
	RenderSystem(db, renders);
}
