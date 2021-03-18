#pragma once

#include <utility/casts.h>

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

	int32_t State() const;
	template <typename T>
	T State() const { return static_cast<T>(State()); }
	int32_t PreviousState() const;
	template <typename T>
	T PreviousState() const { return static_cast<T>(PreviousState()); }

	void AddState(int32_t id);
	template <typename T>
	void AddState(T id) { AddState(to_i32(id)); }

	void SetBeginCallback(int32_t st, std::function<void(StateMachine const&)> const& f);
	void SetUpdateCallback(int32_t st, std::function<int32_t(StateMachine const&)> const& f);
	void SetEndCallback(int32_t st, std::function<void(StateMachine const&)> const& f);

	void ChangeState(int32_t st);
	template <typename T>
	void ChangeState(T id) { ChangeState(to_i32(id)); }
	
	bool Update();
};