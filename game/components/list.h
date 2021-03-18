#pragma once

#include <game/map.h>

#include <components/renderable.h>
#include <components/tiling.h>

#include <entt/entt.hpp>

/*
using AllCompList = entt::type_list<
	SimpleSpriteData,
	TilableSpriteData,
	ComplexSpriteData,
	GridElem
>;
*/

using LevelCompList = entt::type_list<
	SimpleSpriteData,
	TilableSpriteData,
	ComplexSpriteData,
	GridElem
>;

using EditorCompList = LevelCompList;
