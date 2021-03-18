#include <engine/state_machine.h>

StateMachine::StateMachine() {
	state.emplace(-1, Node{ nullptr, nullptr });
}


int32_t StateMachine::State() const {
	return currentState;
}

int32_t StateMachine::PreviousState() const {
	return previousState;
}


void StateMachine::AddState(int32_t id) {
	state.emplace(id, Node());
}


void StateMachine::SetBeginCallback(int32_t st, std::function<void(StateMachine const&)> const& f) {
	state[st].BeginCallback = f;
}

void StateMachine::SetUpdateCallback(int32_t st, std::function<int32_t(StateMachine const&)> const& f) {
	state[st].UpdateCallback = f;
}

void StateMachine::SetEndCallback(int32_t st, std::function<void(StateMachine const&)> const& f) {
	state[st].EndCallback = f;
}


void StateMachine::ChangeState(int32_t st) {
	if (currentState == st) {
		return;
	}
	previousState = currentState;
	currentState = st;
	if (state[previousState].EndCallback) {
		state[previousState].EndCallback(*this);
	}
	if (state[currentState].BeginCallback) {
		state[currentState].BeginCallback(*this);
	}
}

bool StateMachine::Update() {
	if (state[currentState].UpdateCallback) {
		int32_t newState = state[currentState].UpdateCallback(*this);
		if (newState >= 0) {
			bool changed = newState != currentState;
			ChangeState(newState);
			return changed;
		}
	}
	return false;
}
