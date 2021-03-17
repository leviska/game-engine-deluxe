#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class EditorInfoImpl {
public:
	std::vector<std::string> SpriteNames;

	void Load();
	void Reset();
private:

	void LoadSpriteNames();
};

const EditorInfoImpl& EditorInfo();
EditorInfoImpl& EditorInfoMut();
