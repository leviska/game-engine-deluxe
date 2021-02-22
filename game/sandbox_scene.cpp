#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"
#include "resources.h"

#include "imgui.h"

#include <fstream>
#include <queue>

void SandboxScene::Load() {
	LoadMap(map, db, render, std::string("sandboxLevel"));
	
	//lights.emplace_back(259, 152);
	//lights.emplace_back(81, 144);
	//lights.emplace_back(158, 32);

	frameBuffer.Load(Resources().CanvasSize);
	tmpBuffer.Load(Resources().CanvasSize);

	lightTexture.Load(Resources().CanvasSize, true, false);
}

void SandboxScene::Reset() {
	map.clear();
	db.clear();
	frameBuffer.Reset();
	tmpBuffer.Reset();
	render.Reset();

	lightTexture.Reset();
	lights.clear();
}

void SandboxScene::Lights() {
	glm::ivec2 lightPos;
	SDL_GetMouseState(&(lightPos.x), &(lightPos.y));
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
	for (int i = 0; i < 110; i++) {
		glDispatchCompute(384 / 8, 216 / 8, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	Resources().GetShader("LightShader").Select();
	glBindImageTexture(1, lightTexture.GetId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);
	Resources().GetShader("LightShader").SetFloat("Time", time);
	Resources().GetShader("LightShader").SetInt32("LightsSize", static_cast<int>(lights.size()));
	Resources().GetShader("LightShader").SetIVec2Vec("Lights", lights);

	lights.pop_back();
	/*if (!Game().GetWindow().PressedMouse1) {
		lights.pop_back();
	}
	if (!lights.empty() && Game().GetWindow().PressedMouse2) {
		lights.pop_back();
	}*/
}

void SandboxScene::Update() {
	frame++;
	time += Game().FPS().dt / 1000000.f;
}

void SandboxScene::Clear() {
	//ClearRenders(renders);
	//obstructRender.Clear();

	frameBuffer.Select();
	//frameBuffer.Clear({ 39, 39, 68, 0 });
	frameBuffer.Clear({ 31, 14, 28, 0 });
	//frameBuffer.Clear({ 255, 255, 255, 0 });
	tmpBuffer.Select();
	//tmpBuffer.Clear({ 20, 20, 34, 255 });
	tmpBuffer.Clear({ 0, 0, 0, 255 });
	FrameBuffer::SelectWindow();
}

void SandboxScene::Draw() {
	// --------- framebuffer --------- 
	frameBuffer.Select();
	render.Draw();

	// --------- tmpBuffer --------- 
	tmpBuffer.Select();
	
	Lights();

	frameBuffer.Draw({ 0, 0 }, 1, Resources().GetShader("LightShader"));
	
	// --------- screen --------- 
	FrameBuffer::SelectWindow();
	tmpBuffer.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), Resources().GetShader("BufferShader"));
}
