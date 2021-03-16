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

template<typename Func>
struct CallForExistComp {
	CallForExistComp(Func f) : func(f) {}

	template<typename Comp>
	void operator()(entt::registry& reg, entt::entity id) {
		if constexpr (std::is_empty<Comp>::value) {
			return;
		}
		Comp* ptr = reg.try_get<Comp>(id);
		if (ptr) {
			func(*ptr);
		}
	}
private:
	Func func;
};

}


template<typename... Comp, template<typename... Comp> typename List>
void AddCompFromString(entt::registry& reg, entt::entity id, const std::string& name, List<Comp...>) {
	(MetaHelper::AddOneCompFromString<Comp>(reg, id, name), ...);
}


template<typename Func, typename... Comp, template<typename... Comp> typename List>
void CallForEveryComp(entt::registry& reg, entt::entity id, Func f, List<Comp...>) {
	((f.template operator()<Comp>(reg, id)), ...);
}


template<typename Func, typename... Comp, template<typename... Comp> typename List>
void CallForEveryExistComp(entt::registry& reg, entt::entity id, Func f, List<Comp...> list) {
	CallForEveryComp(reg, id, MetaHelper::CallForExistComp(f), list);
}
