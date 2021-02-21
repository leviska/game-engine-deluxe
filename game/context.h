#pragma once

#include "window.h" 
#include "renderer.h"
#include "game.h"

#include <entt/entt.hpp>

struct Context {
	GameInst* game = nullptr;
	Window* window = nullptr;
	Renderer* Render = nullptr;
	entt::registry* Reg = nullptr;
};
