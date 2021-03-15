#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

namespace MetaHelper {

template<typename Comp>
void AddOneCompFromString(entt::registry& reg, entt::entity id, const std::string& name) {
	if (Comp::ComponentName == name) {
		reg.emplace<Comp>(id);
	}
}

template<typename Func, typename Comp>
void CallFunctionForOneComp(entt::registry& reg, entt::entity id, Func f) {
	if constexpr (std::is_empty<Comp>::value) {
		return;
	}
	Comp* ptr = reg.try_get<Comp>(id);
	if (ptr) {
		f(*ptr);
	}
}

template<typename Func, typename Comp>
void CreateCompIfTrue(entt::registry& reg, entt::entity id, Func f) {
	if (f(Comp::ComponentName)) {
		reg.emplace<Comp>(id);
	}
}

}

template<typename... Comp>
void AddCompFromString(entt::registry& reg, entt::entity id, const std::string& name) {
	(MetaHelper::AddOneCompFromString<Comp>(reg, id, name), ...);
}

template<typename... Comp, template<typename... Comp> typename List>
void AddCompFromString(entt::registry& reg, entt::entity id, const std::string& name, List<Comp...>) {
	AddCompFromString<Comp...>(reg, id, name);
}


template<typename Func, typename... Comp>
void CallForEveryComp(entt::registry& reg, entt::entity id, Func f) {
	(MetaHelper::CallFunctionForOneComp<Func, Comp>(reg, id, f), ...);
}

template<typename Func, typename... Comp, template<typename... Comp> typename List>
void CallForEveryComp(entt::registry& reg, entt::entity id, Func f, List<Comp...>) {
	CallForEveryComp<Func, Comp...>(reg, id, f);
}
