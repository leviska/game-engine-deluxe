#include "input.h"

#include "imgui.h"

#include <iostream>

KeyManager::KeyManager(size_t size) {
	prev.resize(size);
	cur.resize(size);
}


bool KeyManager::KeyDown(size_t id) const {
	return cur[id];
}

bool KeyManager::KeyPressed(size_t id) const {
	return !prev[id] && cur[id];
}

bool KeyManager::KeyReleased(size_t id) const {
	return prev[id] && !cur[id];
}

void KeyManager::Tick() {
	prev = cur;
}

void KeyManager::Update(size_t id, bool up) {
	cur[id] = up;
}


const std::unordered_map<SDL_KeyCode, Keyboard> SDLKEYMAP{
	{ SDL_KeyCode::SDLK_q, Keyboard::Q },
	{ SDL_KeyCode::SDLK_w, Keyboard::W },
	{ SDL_KeyCode::SDLK_e, Keyboard::E },
	{ SDL_KeyCode::SDLK_r, Keyboard::R },
	{ SDL_KeyCode::SDLK_t, Keyboard::T },
	{ SDL_KeyCode::SDLK_y, Keyboard::Y },
	{ SDL_KeyCode::SDLK_u, Keyboard::U },
	{ SDL_KeyCode::SDLK_i, Keyboard::I },
	{ SDL_KeyCode::SDLK_o, Keyboard::O },
	{ SDL_KeyCode::SDLK_p, Keyboard::P },
	{ SDL_KeyCode::SDLK_a, Keyboard::A },
	{ SDL_KeyCode::SDLK_s, Keyboard::S },
	{ SDL_KeyCode::SDLK_d, Keyboard::D },
	{ SDL_KeyCode::SDLK_f, Keyboard::F },
	{ SDL_KeyCode::SDLK_g, Keyboard::G },
	{ SDL_KeyCode::SDLK_h, Keyboard::H },
	{ SDL_KeyCode::SDLK_j, Keyboard::J },
	{ SDL_KeyCode::SDLK_k, Keyboard::K },
	{ SDL_KeyCode::SDLK_l, Keyboard::L },
	{ SDL_KeyCode::SDLK_z, Keyboard::Z },
	{ SDL_KeyCode::SDLK_x, Keyboard::X },
	{ SDL_KeyCode::SDLK_c, Keyboard::C },
	{ SDL_KeyCode::SDLK_v, Keyboard::V },
	{ SDL_KeyCode::SDLK_b, Keyboard::B },
	{ SDL_KeyCode::SDLK_n, Keyboard::N },
	{ SDL_KeyCode::SDLK_m, Keyboard::M },
	{ SDL_KeyCode::SDLK_1, Keyboard::KEY1 },
	{ SDL_KeyCode::SDLK_2, Keyboard::KEY2 },
	{ SDL_KeyCode::SDLK_3, Keyboard::KEY3 },
	{ SDL_KeyCode::SDLK_4, Keyboard::KEY4 },
	{ SDL_KeyCode::SDLK_5, Keyboard::KEY5 },
	{ SDL_KeyCode::SDLK_6, Keyboard::KEY6 },
	{ SDL_KeyCode::SDLK_7, Keyboard::KEY7 },
	{ SDL_KeyCode::SDLK_8, Keyboard::KEY8 },
	{ SDL_KeyCode::SDLK_9, Keyboard::KEY9 },
	{ SDL_KeyCode::SDLK_0, Keyboard::KEY0 },
	{ SDL_KeyCode::SDLK_F1, Keyboard::F1 },
	{ SDL_KeyCode::SDLK_F2, Keyboard::F2 },
	{ SDL_KeyCode::SDLK_F3, Keyboard::F3 },
	{ SDL_KeyCode::SDLK_F4, Keyboard::F4 },
	{ SDL_KeyCode::SDLK_F5, Keyboard::F5 },
	{ SDL_KeyCode::SDLK_F6, Keyboard::F6 },
	{ SDL_KeyCode::SDLK_F7, Keyboard::F7 },
	{ SDL_KeyCode::SDLK_F8, Keyboard::F8 },
	{ SDL_KeyCode::SDLK_F9, Keyboard::F9 },
	{ SDL_KeyCode::SDLK_F10, Keyboard::F10 },
	{ SDL_KeyCode::SDLK_F11, Keyboard::F11 },
	{ SDL_KeyCode::SDLK_F12, Keyboard::F12 },
	{ SDL_KeyCode::SDLK_ESCAPE, Keyboard::ESC },
	{ SDL_KeyCode::SDLK_KP_ENTER, Keyboard::ENTER },
	{ SDL_KeyCode::SDLK_KP_TAB, Keyboard::TAB },
	{ SDL_KeyCode::SDLK_LCTRL, Keyboard::LCTRL },
	{ SDL_KeyCode::SDLK_LSHIFT, Keyboard::LSHIFT },
	{ SDL_KeyCode::SDLK_LALT, Keyboard::LALT },
};


InputImpl::InputImpl()
	: keyboard(static_cast<size_t>(Keyboard::KeyboardSize))
	, mouse(static_cast<size_t>(Mouse::MouseSize)) {}

void InputImpl::Load() {
	// TODO: load keymap
}

void InputImpl::Reset() {
	// TODO: reset keymap
}


void InputImpl::Tick() {
	keyboard.Tick();
	mouse.Tick();
}

void InputImpl::Update(const SDL_Event& event) {
	auto updateKey = [&](bool value) {
		auto it = SDLKEYMAP.find(static_cast<SDL_KeyCode>(event.key.keysym.sym));
		if (it != SDLKEYMAP.end()) {
			Keyboard key = it->second;
			keyboard.Update(static_cast<size_t>(key), value);
		}
	};

	switch (event.type) {
	case SDL_KEYDOWN: {
		updateKey(true);
		break;
	}
	case SDL_KEYUP: {
		updateKey(false);
		break;
	}
	case SDL_MOUSEBUTTONDOWN: {
		auto button = event.button.button;
		if (button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT) {
			if (!ImGui::GetIO().WantCaptureMouse) {
				mouse.Update(button == SDL_BUTTON_LEFT ? 0 : 1, true);
			}
		}
		break;
	}
	case SDL_MOUSEBUTTONUP: {
		auto button = event.button.button;
		if (button == SDL_BUTTON_LEFT || button == SDL_BUTTON_RIGHT) {
			mouse.Update(button == SDL_BUTTON_LEFT ? 0 : 1, false);
		}
		break;
	}
	}
}


bool InputImpl::KeyDown(Keyboard key) const {
	return keyboard.KeyDown(static_cast<size_t>(key));
}

bool InputImpl::KeyPressed(Keyboard key) const {
	return keyboard.KeyPressed(static_cast<size_t>(key));
}


bool InputImpl::KeyDown(Mouse key) const {
	return mouse.KeyDown(static_cast<size_t>(key));
}

bool InputImpl::KeyPressed(Mouse key) const {
	return mouse.KeyPressed(static_cast<size_t>(key));
}


bool InputImpl::KeyDown(Gamepad key) const {
	return false; // TODO
}

bool InputImpl::KeyPressed(Gamepad key) const {
	return false; // TODO
}


bool InputImpl::KeyDown(GameKey key) const {
	const auto& kp = keymap.at(key);
	return KeyDown(kp.Key) || KeyDown(kp.Pad);
}

bool InputImpl::KeyPressed(GameKey key) const {
	const auto& kp = keymap.at(key);
	return KeyPressed(kp.Key) || KeyPressed(kp.Pad);
}

const InputImpl& Input() {
	return InputMut();
}

InputImpl& InputMut() {
	static InputImpl ref;
	return ref;
}
