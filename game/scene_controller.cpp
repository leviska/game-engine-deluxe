#include "scene_controller.h"

void SceneController::Load() {
	levelEditor.Load();
	sandbox.Load();

	stateMap = {
		{ Scenes::Sandbox, &sandbox },
		{ Scenes::LevelEditor, &levelEditor },
	};

	states.AddState(static_cast<int32_t>(Scenes::Sandbox));
	states.AddState(static_cast<int32_t>(Scenes::LevelEditor));
	states.ChangeState(static_cast<int32_t>(Scenes::Sandbox));
}

void SceneController::Reset() {
	levelEditor.Reset();
	sandbox.Reset();

	stateMap.clear();
	states = StateMachine();
}


Scenes SceneController::CurrentState() {
	return static_cast<Scenes>(states.State());
}

Scene& SceneController::CurrentScene() {
	return *stateMap[CurrentState()];
}


const StateMachine& SceneController::SceneState() const {
	return states;
}

StateMachine& SceneController::SceneState() {
	return states;
}