#pragma once

#include <game/map.h>

#include <components/movable.h>
#include <components/renderable.h>
#include <components/obstructable.h>
#include <components/tilable.h>

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
	ComplexSpriteData,
	TilableData,
	TilableRender,
	InputMovableFlag,
	FollowCameraFlag,
	ObstructFlag,
	CheckForObstructFlag,
	GridElem
>;

using EditorCompList = LevelCompList;
