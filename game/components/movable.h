#pragma once

#include <game/map.h>

#include <glm/glm.hpp>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

struct GridElem {
	glm::ivec2 Pos;

	void ClearGridElem(entt::entity id, MapView& map);
	void UpdateGridElem(entt::entity id, MapView& map);


	inline static const std::string ComponentName{ "GridElem" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};

struct MoveEntity {
	glm::ivec2 Shift;
};

struct InputMovableFlag {
	inline static const std::string ComponentName{ "InputMovableFlag" };
};

struct InputMovableEditorFlag {};

struct FollowCameraFlag {
	inline static const std::string ComponentName{ "FollowCameraFlag" };
};
