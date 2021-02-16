#pragma once

#include "rendering.h"

#include <entt/entt.hpp>

struct Context {
	Rendering* Render = nullptr;
	entt::registry* Reg = nullptr;
};