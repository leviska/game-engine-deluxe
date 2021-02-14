#include "state_machine.h"

StateMachine::StateMachine() {
	state.emplace(-1, Node{ nullptr, nullptr });
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
