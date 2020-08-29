#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"
#include "resources.h"

#include "imgui.h"

#include <fstream>
#include <queue>

void SandboxScene::Load() {
	renders[0].Load(0);
	map.Load(db);
	LoadMap(map, "sandboxLevel");

	obstructRender.Load(0, static_cast<uint32_t>(Shaders::Obstruct));
	frameBuffer.Load(Resources().CanvasSize);
	tmpBuffer.Load(Resources().CanvasSize);

	//lightImage.Load(Resources().CanvasSize, { 255, 255, 255, 0 });
	lightTexture.Load(Resources().CanvasSize, true, false);

	entt::entity id = db.create();
	Sprite sprite("SkeletonRightStand00");
	sprite.Pos = glm::uvec2(4, 4) * Resources().TileSize;
	sprite.Pos += glm::vec2{ Resources().TileSize / 2, Resources().TileSize / 2 };
	db.emplace<Renderable>(id, sprite);

	//Lights();
}

void SandboxScene::Reset() {
	map.Reset();
	db.clear();
	frameBuffer.Reset();
	tmpBuffer.Reset();
	obstructRender.Reset();

	lightTexture.Reset();
	lightImage.Reset();
	ResetRenders(renders);
}

void SandboxScene::Lights() {
	glm::ivec2 lightPos;
	uint32_t buttons = SDL_GetMouseState(&(lightPos.x), &(lightPos.y));
	lightPos -= Game().GetWindow().GetSize() / 2u;
	uint32_t scaledTile = Game().GetScale();
	lightPos /= static_cast<int32_t>(scaledTile);
	lightPos += Resources().CanvasSize / 2u;
	lightPos = glm::clamp(lightPos, { 0, 0 }, glm::ivec2{ Resources().CanvasSize } -glm::ivec2(1, 1));

	//lightPos = glm::ivec2(9 * 16, 3 * 16);
	lights.push_back(lightPos);


	Shader& genShader = Resources().GetShader("GenerateLight");
	genShader.Select();

	glBindImageTexture(0, frameBuffer.GetTexture().GetId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8UI);
	glBindImageTexture(1, lightTexture.GetId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
	genShader.SetInt32("LightsSize", static_cast<int>(lights.size()));
	genShader.SetIVec2Vec("Lights", lights);

	glDispatchCompute(384 / 8, 216 / 8, 1);
	genShader.SetInt32("LightsSize", 0);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	for (int i = 0; i < 150; i++) {
		glDispatchCompute(384 / 8, 216 / 8, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}



	if (!ImGui::GetIO().WantCaptureMouse && (buttons & SDL_BUTTON(SDL_BUTTON_LEFT))) {
		if (pressedL) {
			lights.pop_back();
		}
		pressedL = true;
	}
	else {
		pressedL = false;
		lights.pop_back();
	}
	if (!ImGui::GetIO().WantCaptureMouse && (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))) {
		if (!pressedR) {
			lights.pop_back();
		}
		pressedR = true;
	}
	else {
		pressedR = false;
	}
}

void SandboxScene::Update() {
}

void SandboxScene::Clear() {
	ClearRenders(renders);
	obstructRender.Clear();

	frameBuffer.Select();
	//frameBuffer.Clear({ 39, 39, 68, 0 });
	frameBuffer.Clear({ 56, 58, 97, 0 });
	//frameBuffer.Clear({ 255, 255, 255, 0 });
	tmpBuffer.Select();
	//tmpBuffer.Clear({ 20, 20, 34, 255 });
	tmpBuffer.Clear({ 0, 0, 0, 255 });
	FrameBuffer::SelectWindow();
}

void SandboxScene::Draw() {
	// --------- framebuffer --------- 
	frameBuffer.Select();
	RenderSystem(db, renders);

	// --------- tmpBuffer --------- 
	tmpBuffer.Select();
	
	Lights();

	Resources().GetShader("LightShader").Select();
	glBindImageTexture(1, lightTexture.GetId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);

	frameBuffer.Draw({ 0, 0 }, 1, Resources().GetShader("LightShader"));
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
	if (frame++ == 30) {
		lightTexture.Save().Save("log/tmppp.png");
	}
	// --------- screen --------- 
	FrameBuffer::SelectWindow();
	tmpBuffer.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), Resources().GetShader("BufferShader"));
}
