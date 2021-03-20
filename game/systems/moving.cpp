#include <systems/moving.h>

#include <components/movable.h>
#include <components/renderable.h>

#include <engine/input.h>

template <typename T>
void MoveByInput(entt::registry& reg, const Keys<4>& keys) {
	auto view = reg.view<T>();
	for (auto id : view) {
		reg.emplace<MoveEntity>(id, Input().DirectionPressed(keys));
		reg.emplace_or_replace<UpdateRendPositionFlag>(id);
	}
}

void UpdateMoving(entt::registry& reg) {
	MoveByInput<InputMovableFlag>(reg, WASD);
}

void UpdateMovingEditor(entt::registry& reg) {
	MoveByInput<InputMovableEditorFlag>(reg, ARROWS);
}


void MoveEntities(entt::registry& reg, MapView& map) {
	auto view = reg.view<GridElem, MoveEntity>();
	for (auto id : view) {
		auto [grid, dir] = view.get(id);
		grid.ClearGridElem(id, map);
		grid.Pos += dir.Shift;
		grid.UpdateGridElem(id, map);
		reg.remove<MoveEntity>(id);
	}
}


void UpdateFollowCamera(entt::registry& reg) {
	auto cameraView = reg.view<Camera>();
	auto followView = reg.view<GridElem, FollowCameraFlag>();
	for (auto cameraId : cameraView) {
		auto [camera] = cameraView.get(cameraId);
		for (auto id : followView) {
			auto [grid] = followView.get(id);
			camera.Pos = GetSpritePos(grid.Pos);
		}
	}
}