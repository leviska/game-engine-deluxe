#include "camera.h"

#include "input.h"
#include "consts.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 pos, glm::vec2 size) : Pos(pos), Size(size) {}

glm::mat4 Camera::Matrix() const {
	glm::vec2 a = Pos - Size;
	glm::vec2 b = Pos + Size;
	return glm::ortho(a.x, b.x, a.y, b.y, -1.0f, Consts().MaxLayers + 1.0f);
}

void Camera::UpdateFreeCamera() {
	if (Input().KeyDown(Keyboard::W)) {
		Pos.y--;
	}
	if (Input().KeyDown(Keyboard::S)) {
		Pos.y++;
	}
	if (Input().KeyDown(Keyboard::A)) {
		Pos.x--;
	}
	if (Input().KeyDown(Keyboard::D)) {
		Pos.x++;
	}
}


glm::ivec2 GlobalToGamePos(glm::ivec2 pos) {
	glm::ivec2 centerShift = Consts().WindowSize / (Consts().Scale * 2) - (Consts().CanvasSize >> 1u);
	return pos / static_cast<int32_t>(Consts().Scale) - centerShift;
}

glm::ivec2 GlobalToGamePos(glm::ivec2 pos, const Camera& camera) {
	return GlobalToGamePos(pos) - glm::ivec2(camera.Size - camera.Pos);
}

glm::ivec2 GlobalToGridPos(glm::ivec2 pos, const Camera& camera) {
	return glm::floor(glm::vec2(GlobalToGamePos(pos, camera)) / glm::vec2(Consts().TileSize));
}

glm::ivec2 GameToGlobalPos(glm::ivec2 pos) {
	glm::ivec2 centerShift = Consts().WindowSize / (Consts().Scale * 2) - (Consts().CanvasSize >> 1u);
	return (pos + centerShift) * static_cast<int32_t>(Consts().Scale);
}

glm::ivec2 GameToGlobalPos(glm::ivec2 pos, const Camera& camera) {
	return GameToGlobalPos(pos + glm::ivec2(camera.Size - camera.Pos));
}

glm::ivec2 GridToGlobalPos(glm::ivec2 pos, const Camera& camera) {
	return GameToGlobalPos(pos * static_cast<int32_t>(Consts().TileSize), camera);
}
