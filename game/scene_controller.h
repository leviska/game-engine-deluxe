#pragma once

#include "state_machine.h"

#include <unordered_map>

// Scenes
#include "level_scene.h"

#include "level_editor.h"
#include "sandbox_scene.h"

enum class Scenes {
	/*
	Menu, 
	Level,
	Cutscene,
	Dialog,
	*/
	Sandbox,
	LevelEditor,

	ScenesSize
};

class SceneController {
public:
	void Load();
	void Reset();

	Scenes CurrentState();
	Scene& CurrentScene();

	const StateMachine& SceneState() const;
	StateMachine& SceneState();
private:
	// TBA MenuScene
	// TBA LevelScene
	// TBA CutScene
	// TBA DialogScene
	LevelEditorScene levelEditor;
	SandboxScene sandbox;

	StateMachine states;

	std::unordered_map<Scenes, Scene*> stateMap;
};