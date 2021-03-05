#include "scene_controller.h"

void SceneController::Load() {
	level.Load("sandboxLevel");
	levelEditor.Load();
	sandbox.Load();

	stateMap = {
		{ Scenes::Level, &level },
		{ Scenes::Sandbox, &sandbox },
		{ Scenes::LevelEditor, &levelEditor },
	};

	states.AddState(Scenes::Level);
	states.AddState(Scenes::Sandbox);
	states.AddState(Scenes::LevelEditor);
	states.ChangeState(Scenes::Level);
}

void SceneController::Reset() {
	levelEditor.Reset();
	sandbox.Reset();

	stateMap.clear();
	states = StateMachine();
}


Scene& SceneController::CurrentScene() {
	return *stateMap[states.State<Scenes>()];
}


void SceneController::LoadLevel(const std::string& name) {
	levelEditor.Reset();
	levelEditor.Load(name);
	
	level.Reset();
	level.Load(name);
}


const StateMachine& SceneController::SceneState() const {
	return states;
}

StateMachine& SceneController::SceneState() {
	return states;
}


LevelScene& SceneController::Level() {
	return level;
}

LevelEditorScene& SceneController::LevelEditor() {
	return levelEditor;
}

SandboxScene& SceneController::Sandbox() {
	return sandbox;
}