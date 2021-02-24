#pragma once

#include <sdl2/SDL.h>

#include <vector>
#include <unordered_map>

enum class Keyboard {
	Q, W, E, R, T, Y, U, I, O, P, // 1 row
	A, S, D, F, G, H, J, K, L, // 2 row
	Z, X, C, V, B, N, M, // 3 row
	KEY1, KEY2, KEY3, KEY4, KEY5, KEY6, KEY7, KEY8, KEY9, KEY0, // numbers
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, // functional keys
	ESC, ENTER, TAB, // esc
	LCTRL, LSHIFT, LALT, // control keys
	KeyboardSize // total keys
};

enum class Mouse {
	Left, Right,
	MouseSize
};

enum class Gamepad {
	// TODO
	GamepadSize // total keys
};

enum class GameKey {
	Top, Right, Bottom, Left, // movement
	GameKeySize // total keys
};

class KeyManager {
public:
	KeyManager(size_t size);

	bool KeyDown(size_t id) const;
	bool KeyPressed(size_t id) const;
	bool KeyReleased(size_t id) const;

	void Tick();
	void Update(size_t id, bool up);
private:
	std::vector<bool> prev;
	std::vector<bool> cur;
};

class InputImpl {
public:
	InputImpl();

	void Load();
	void Reset();

	void Tick();
	void Update(const SDL_Event& event);

	bool KeyDown(Keyboard key) const;
	bool KeyPressed(Keyboard key) const;

	bool KeyDown(Mouse key) const;
	bool KeyPressed(Mouse key) const;

	bool KeyDown(Gamepad key) const;
	bool KeyPressed(Gamepad key) const;

	bool KeyDown(GameKey key) const;
	bool KeyPressed(GameKey key) const;
private:
	struct KeyPair {
		Keyboard Key;
		Gamepad Pad;
	};

	KeyManager keyboard;
	KeyManager mouse;
	std::unordered_map<GameKey, KeyPair> keymap;
};

const InputImpl& Input();
InputImpl& InputMut();
