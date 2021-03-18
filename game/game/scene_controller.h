#pragma once

#include <engine/state_machine.h>

// Scenes
#include <game/level_scene.h>
#include <game/editor.h>
#include <game/sandbox_scene.h>

#include <unordered_map>

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