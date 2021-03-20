#include <systems/obstructing.h>

#include <components/obstructable.h>
#include <components/movable.h>

void CheckForObstructing(entt::registry& reg, MapView& map) {
	auto view = reg.view<GridElem, MoveEntity, CheckForObstructFlag>();
	for (auto id : view) {
		auto [grid, move] = view.get(id);
		glm::ivec2 to = grid.Pos + move.Shift;
		entt::entity toObstr = FirstTypeInMap<ObstructFlag>(to, map, reg);
		if (toObstr != entt::null) {
			move.Shift = glm::ivec2(0, 0);
		}
	}
}