#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"
#include "resources.h"

#include <fstream>

void SandboxScene::Load() {
	renders[0].Load(0);
	map.Load(db);
	LoadMap(map, "sandboxLevel");

	obstructRender.Load(0, static_cast<uint32_t>(Shaders::Obstruct));
	frameBuffer.Load(Resources().CanvasSize);
	tmpBuffer.Load(Resources().CanvasSize);
	obstructBuffer.Load(Resources().CanvasSize);
	
	entt::entity id = db.create();
	Sprite sprite;
	sprite.Load("SkeletonRightStand00");
	sprite.Pos = glm::uvec2(4, 4) * Resources().TileSize;
	sprite.Pos += glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
	db.emplace<Renderable>(id, sprite);
}

void SandboxScene::Reset() {
	db.clear();
	map.Reset();
	frameBuffer.Reset();
	tmpBuffer.Reset();
	obstructBuffer.Reset();
	obstructRender.Reset();
	ResetRenders(renders);
}

void SandboxScene::Update() {}

void SandboxScene::Clear() {
	ClearRenders(renders);
	obstructRender.Clear();

	frameBuffer.Select();
	frameBuffer.Clear(glm::vec4(39.0f / 255.0f, 39.0f / 255.0f, 68.0f / 255.0f, 1.0f));
	tmpBuffer.Select();
	tmpBuffer.Clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	obstructBuffer.Select();
	obstructBuffer.Clear(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	FrameBuffer::SelectWindow();
}

void SandboxScene::Draw() {
	frameBuffer.Select();
	RenderSystem(db, renders);
	obstructBuffer.Select();
	RenderObstructSystem(db, obstructRender);
	tmpBuffer.Select();
	obstructBuffer.GetTexture().Select(1);
	static float time = 0;
	time += 0.001;
	Resources().GetShader("LightShader").Select();
	Resources().GetShader("LightShader").SetFloat("Time", time);
	frameBuffer.Draw({ 0, 0 }, 1, Resources().GetShader("LightShader"));
	FrameBuffer::SelectWindow();
	tmpBuffer.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), Resources().GetShader("BufferShader"));
}
