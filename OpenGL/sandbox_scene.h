#pragma once

#include "scene.h"

#include "batching.h"
#include "particles.h"
#include "state_machine.h"
#include "map.h"
#include "framebuffer.h"

#include "entt/entt.hpp"

class SandboxScene : public Scene {
public:
	FrameBuffer frameBuffer;
	FrameBuffer tmpBuffer;

	Texture lightTexture;
	Image lightImage;
	std::vector<glm::ivec2> lights;

	bool pressedL = false;
	bool pressedR = false;

	std::unordered_map<uint32_t, BatchedRender> renders;
	BatchedRender obstructRender;


	entt::registry db;
	WallMap map;

	void Load();
	void Reset() override;

	void Lights(const Image& obstructs);
	
	void Update() override;
	void Clear() override;
	void Draw() override;
};
