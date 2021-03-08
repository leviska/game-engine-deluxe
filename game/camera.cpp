#include "camera.h"

#include "input.h"

Camera::Camera(glm::vec2 pos, glm::vec2 size) : Pos(pos), Size(size) {}

glm::mat4 Camera::Matrix() const {
	glm::vec2 a = Pos - Size;
	glm::vec2 b = Pos + Size;
	return glm::ortho(a.x, b.x, a.y, b.y, -3.0f, 3.0f);
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
