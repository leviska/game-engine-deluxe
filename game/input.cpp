#include "input.h"


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
};


InputImpl::InputImpl() : keyboard(static_cast<size_t>(Keyboard::KEYBOARD_SIZE)) {}

void InputImpl::Load() {
	// TODO: load keymap
}

void InputImpl::Reset() {
	// TODO: reset keymap
}


void InputImpl::Tick() {
	keyboard.Tick();
}

void InputImpl::Update(const SDL_Event& event) {
	switch (event.type) {
	case SDL_KEYDOWN: {
		Keyboard key = SDLKEYMAP.at(static_cast<SDL_KeyCode>(event.key.keysym.sym));
		keyboard.Update(static_cast<size_t>(key), false);
		break;
	}
	case SDL_KEYUP: {
		Keyboard key = SDLKEYMAP.at(static_cast<SDL_KeyCode>(event.key.keysym.sym));
		keyboard.Update(static_cast<size_t>(key), true);
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
