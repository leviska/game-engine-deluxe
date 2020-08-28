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

	lightImage.Load(Resources().CanvasSize, { 255, 255, 255, 0 });
	lightTexture.Load(lightImage);

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

void SandboxScene::Lights(const Image& obstructs) {
	for (size_t x = 0; x < lightImage.GetSize().x; x++) {
		for (size_t y = 0; y < lightImage.GetSize().y; y++) {
			lightImage[{x, y}].r = obstructs[{x, y}].a;
			lightImage[{x, y}].a = 0;
		}
	}
	const glm::ivec2 shifts[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 1 }, {-1, 1}, {-1, -1}, {1, -1} };
	std::vector<std::vector<float>> t(Resources().CanvasSize.x, std::vector<float>(Resources().CanvasSize.y, -1));
	std::queue<glm::ivec2> q;

	glm::ivec2 lightPos;
	uint32_t buttons = SDL_GetMouseState(&(lightPos.x), &(lightPos.y));
	lightPos -= Game().GetWindow().GetSize() / 2u;
	uint32_t scaledTile = Game().GetScale();
	lightPos /= static_cast<int32_t>(scaledTile);
	lightPos += Resources().CanvasSize / 2u;
	lightPos = glm::clamp(lightPos, { 0, 0 }, glm::ivec2{ Resources().CanvasSize } - glm::ivec2(1, 1));

	//lightPos = glm::ivec2(9 * 16, 3 * 16);
	lights.push_back(lightPos);

	for (const auto& i : lights) {
		q.push(i);
		lightImage[i].a = 255;
		t[i.x][i.y] = 150;
	}

	while (!q.empty()) {
		glm::ivec2 cur = q.front();
		q.pop();

		float curA = obstructs[cur].a;
		for (auto i : shifts) {
			glm::ivec2 pos = cur + i;
			if (pos.x < 0 || pos.y < 0 || pos.x >= Resources().CanvasSize.x || pos.y >= Resources().CanvasSize.y) {
				continue;
			}
			float curValue = t[cur.x][cur.y] - glm::length(glm::vec2(i));
			if (t[pos.x][pos.y] >= curValue - 0.1) {
				continue;
			}
			if (curA > 0 && obstructs[pos].a == 0) {
				continue;
			}
			if (curValue <= 0) {
				continue;
			}

			t[pos.x][pos.y] = curValue;
			lightImage[pos].a = std::min(255.0f, curValue * 255 / 150);

			q.push(pos);
		}
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
	//lightImage.Save("tmp.png");
}

void SandboxScene::Update() {
	//Lights();
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
	frameBuffer.Select();
	RenderSystem(db, renders);

	Image tmp = frameBuffer.GetTexture().Save();
	Lights(tmp);

	tmpBuffer.Select();
	lightTexture.Select(0);
	lightTexture.Update(lightImage);
	lightTexture.Select(1);

	Resources().GetShader("LightShader").Select();
	
	frameBuffer.Draw({ 0, 0 }, 1, Resources().GetShader("LightShader"));

	FrameBuffer::SelectWindow();
	tmpBuffer.Draw(Game().GetWindow().GetSize() / 2u, Game().GetScale(), Resources().GetShader("BufferShader"));
}
