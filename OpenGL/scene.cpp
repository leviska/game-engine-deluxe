#include "scene.h"

#include "game.h"
#include "renderable.h"

#include <fstream>

void SandboxScene::Load() {
	tileRender.Load(0);
	map.Load(db);
	LoadMap(map, 0);
	nlohmann::json a;
	SaveMap(map, a);
	std::ofstream tmp("assets/wdwada.json");
	tmp << a;
}

void SandboxScene::Reset() {
	db.clear();
	tileRender.Reset();
}

void SandboxScene::Update() {
}

void SandboxScene::Clear() {
	tileRender.Clear();
}

void SandboxScene::Draw() {
	RenderSystem(db, tileRender);
	tileRender.Draw();
}
