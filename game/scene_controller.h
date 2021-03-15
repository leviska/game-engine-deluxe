#pragma once

#include "state_machine.h"

#include <unordered_map>

// Scenes
#include "level_scene.h"

#include "editor.h"
#include "sandbox_scene.h"

enum class Scenes {
	Menu, 
	Level,
	Cutscene,
	Dialog,
	
	Sandbox,
	Editor,

	ScenesSize
};

class SceneController {
public:
	void Load();
	void Reset();

	Scene& CurrentScene();

	const StateMachine& SceneState() const;
	StateMachine& SceneState();

	void LoadLevel(const std::string& name);

	LevelScene& Level();
	EditorScene& Editor();
	SandboxScene& Sandbox();
private:
	// TBA MenuScene
	// TBA CutScene
	// TBA DialogScene
	LevelScene level;
	EditorScene editor;
	SandboxScene sandbox;

	StateMachine states;

	std::unordered_map<Scenes, Scene*> stateMap;
};