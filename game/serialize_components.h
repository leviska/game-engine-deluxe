#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

template<typename Comp> 
void LoadSingleComponent(const nlohmann::json& data, entt::registry& reg, entt::entity id) {
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
}

template<typename... Comp>
void LoadComponents(const nlohmann::json& data, entt::registry& reg, entt::entity id) {
	(LoadSingleComponent<Comp>(data, reg, id), ...);
}

template<typename... Comp>
void LoadEntity(const nlohmann::json& data, entt::registry& reg) {
	entt::entity id = reg.create();
	LoadComponents<Comp...>(data, reg, id);
}

template<typename... Comp>
void LoadRegistry(const nlohmann::json& data, entt::registry& reg) {
	for (const auto& value : data) {
		LoadEntity<Comp...>(value, reg);
	}
}

template<typename Registry, typename Comp>
void SaveSingleComponent(nlohmann::json& data, const Registry& reg, const Comp& value) {
	value.Save(data[Comp::ComponentName]);
}

template<typename Registry, typename Comp>
void SaveSingleComponentIfExists(nlohmann::json& data, const Registry& reg, entt::entity id) {
	if (reg.has<Comp>(id)) {
		data[Comp::ComponentName] = nlohmann::json::object();
		if constexpr (!std::is_empty<Comp>::value) {
			const Comp& value = reg.get<Comp>(id);
			SaveSingleComponent<Registry, Comp>(data[Comp::ComponentName], reg, value);
		}
	}
}

template<typename Registry, typename... Comp>
void SaveComponents(nlohmann::json& data, const Registry& reg, entt::entity id) {
	(SaveSingleComponentIfExists<Registry, Comp>(data, reg, id), ...);
}

template<typename Registry, typename... Comp>
void SaveEntity(nlohmann::json& data, const Registry& reg, entt::entity id) {
	nlohmann::json res = nlohmann::json::object();
	SaveComponents<Registry, Comp...>(res, reg, id);
	data.push_back(res);
}

template<typename Registry, typename... Comp>
void SaveRegistry(nlohmann::json& data, const Registry& reg) {
	reg.each([&](entt::entity id) {
		SaveEntity<Registry, Comp...>(data, reg, id);
	});
}
