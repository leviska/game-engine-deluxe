#pragma once

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <functional>

class StateMachine;

using BeginCallbackFunc = std::function<void(StateMachine const&)>;
using UpdateCallbackFunc = std::function<int32_t(StateMachine const&)>;
using EndCallbackFunc = std::function<void(StateMachine const&)>;

struct Node {
	BeginCallbackFunc BeginCallback = nullptr;
	UpdateCallbackFunc UpdateCallback = nullptr;
	EndCallbackFunc EndCallback = nullptr;
};

class StateMachine {
	std::unordered_map<int32_t, Node> state;
	int32_t currentState = -1;
	int32_t previousState = -1;
public:
	StateMachine();

	int32_t State() const { return currentState; }
	int32_t PreviousState() const { return previousState; }

	void AddState(int32_t id) { state.emplace(id, Node()); }

	void SetBeginCallback(int32_t st, std::function<void(StateMachine const&)> const& f) { state[st].BeginCallback = f; }
	void SetUpdateCallback(int32_t st, std::function<int32_t(StateMachine const&)> const& f) { state[st].UpdateCallback = f; }
	void SetEndCallback(int32_t st, std::function<void(StateMachine const&)> const& f) { state[st].EndCallback = f; }

	void ChangeState(int32_t st);
	bool Update();
};