#pragma once

#include <entt/entt.hpp>

#include "renderable.h"
#include "map.h"

using AllCompList = entt::type_list<
	SimpleSpriteData,
	TilableSpriteData,
	ComplexSpriteData,
	GridElem
>;

using LevelCompList = entt::type_list<
	SimpleSpriteData,
	TilableSpriteData,
	ComplexSpriteData,
	GridElem
>;

using EditorCompList = LevelCompList;
