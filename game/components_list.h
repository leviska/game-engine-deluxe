#pragma once

#include <entt/entt.hpp>

#include "renderable.h"
#include "map_components.h"
#include "map.h"

using AllCompList = entt::type_list<
	SimpleSpriteData,
	TilableSpriteData,
	GridElem
>;

using EditorCompList = entt::type_list<
	SimpleSpriteData,
	TilableSpriteData,
	GridElem
>;
