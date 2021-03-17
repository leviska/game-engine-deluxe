#pragma once

#include <entt/entt.hpp>

template<typename Func, typename... Comp, template<typename...> typename List>
void ForEachType(List<Comp...>, Func f) {
	(f(entt::type_identity<Comp>()), ...);
}
