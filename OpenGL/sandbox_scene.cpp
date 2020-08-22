#include "sandbox_scene.h"

#include "game.h"
#include "renderable.h"
#include "resources.h"

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
	Sprite sprite;
	sprite.Load("SkeletonRightStand00");
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
			lightImage[{x, y}].a = obstructs[{x, y}].a;
		}
	}
	glm::ivec2 lightPos;
	SDL_GetMouseState(&(lightPos.x), &(lightPos.y));
	lightPos -= Game().GetWindow().GetSize() / 2u;
	uint32_t scaledTile = Game().GetScale();
	lightPos /= static_cast<int32_t>(scaledTile);
	lightPos += Resources().CanvasSize / 2u;
	lightPos = glm::clamp(lightPos, { 0, 0 }, glm::ivec2{ Resources().CanvasSize } - glm::ivec2(1, 1));
	std::queue<glm::ivec2> q;
	q.push(lightPos);
	lightImage[lightPos].a = 255;
	const glm::ivec2 shifts[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 1 }, {-1, 1}, {-1, -1}, {1, -1} };

	std::vector<std::vector<float>> t(Resources().CanvasSize.x, std::vector<float>(Resources().CanvasSize.y, -1));
	t[lightPos.x][lightPos.y] = 255;
	while (!q.empty()) {
		glm::ivec2 cur = q.front();
		q.pop();

		for (auto i : shifts) {
			glm::ivec2 pos = cur + i;
			if (pos.x < 0 || pos.y < 0 || pos.x >= Resources().CanvasSize.x || pos.y >= Resources().CanvasSize.y) {
				continue;
			}
			if (t[pos.x][pos.y] >= 0) {
				continue;
			}
			if (obstructs[pos].a > 0) {
				continue;
			}
			float curValue = t[cur.x][cur.y] - 1; // glm::length(glm::vec2(i));
			if (curValue <= 0) {
				continue;
			}

			t[pos.x][pos.y] = curValue;
			lightImage[pos].a = curValue;

			q.push(pos);
		}
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
	//frameBuffer.Clear(RGBA({ 39, 39, 68, 0 }));
	frameBuffer.Clear(RGBA({ 255, 255, 255, 0 }));
	tmpBuffer.Select();
	tmpBuffer.Clear(RGBA({ 0, 0, 0, 255 }));
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
