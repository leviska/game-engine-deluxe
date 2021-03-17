#pragma once

#include "components_utility.h"

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>


template<typename... Comp, template<typename...> typename List>
void LoadEntity(List<Comp...>, entt::entity id, const nlohmann::json& data, entt::registry& reg) {
	ForEachType(entt::type_list<Comp...>(), [&](auto type) {
		using Comp = typename decltype(type)::type;

		auto it = data.find(Comp::ComponentName);
		if (it == data.end()) {
			return;
		}
		if constexpr (std::is_empty<Comp>::value) {
			reg.emplace<Comp>(id);
		}
		else {
			reg.emplace<Comp>(id).Load(*it);
		}
	});
}

template<typename... Comp, template<typename...> typename List>
void LoadRegistry(List<Comp...>, const nlohmann::json& data, entt::registry& reg) {
	for (const auto& value : data) {
		entt::entity id = reg.create();
		LoadEntity(List<Comp...>(), id, value, reg);
	}
}


template<typename Registry, typename... Comp, template<typename...> typename List>
nlohmann::json SaveEntity(List<Comp...>, const Registry& reg, entt::entity id) {
	nlohmann::json res = nlohmann::json::object();

	ForEachType(entt::type_list<Comp...>(), [&](auto type) {
		using Comp = typename decltype(type)::type;
		
		if (reg.has<Comp>(id)) {
			res[Comp::ComponentName] = nlohmann::json::object();
			if constexpr (!std::is_empty<Comp>::value) {
				const Comp& value = reg.get<Comp>(id);
				value.Save(res[Comp::ComponentName]);
			}
		}
	});
	return res;
}

template<typename Registry, typename... Comp, template<typename...> typename List>
void SaveRegistry(List<Comp...>, nlohmann::json& data, const Registry& reg) {
	reg.each([&](entt::entity id) {
		data.push_back(SaveEntity(List<Comp...>(), reg, id));
	});
}
