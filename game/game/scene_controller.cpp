#include <game/scene_controller.h>

#include <utility/assertion.h>

void SceneController::Load() {
	DOUT() << "Loading Scenes" << std::endl;

	level.Load("sandboxLevel");
	editor.Load();
	sandbox.Load();

	stateMap = {
		{ Scenes::Level, &level },
		{ Scenes::Sandbox, &sandbox },
		{ Scenes::Editor, &editor },
	};

	states.AddState(Scenes::Level);
	states.AddState(Scenes::Sandbox);
	states.AddState(Scenes::Editor);
	states.ChangeState(Scenes::Level);

	DOUT() << "Successfully loaded Scenes" << std::endl;
}

void SceneController::Reset() {
	editor.Reset();
	sandbox.Reset();

	stateMap.clear();
	states = StateMachine();
}


Scene& SceneController::CurrentScene() {
	return *stateMap[states.State<Scenes>()];
}


void SceneController::LoadLevel(const std::string& name) {
	editor.Reset();
	editor.Load(name);
	
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

EditorScene& SceneController::Editor() {
	return editor;
}

SandboxScene& SceneController::Sandbox() {
	return sandbox;
}