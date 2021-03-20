#pragma once

#include <type_traits>

#include <entt/entt.hpp>

// Primary template handles all types not supporting the operation.
template <typename, template <typename> class, typename = std::void_t<>>
struct detect_v : std::false_type {};

// Specialization recognizes/validates only types supporting the archetype.
template <typename T, template <typename> class Op>
struct detect_v<T, Op, std::void_t<Op<T>>> : std::true_type {};

/*
template<typename T>
using foo_t = decltype(std::declval<T&>().foo(int()));

template<typename T>
constexpr bool has_foo = detect_v<T, foo_t>::value;
*/

template<typename Func, typename... Comp, template<typename...> typename List>
void ForEachType(List<Comp...>, Func f) {
	(f(entt::type_identity<Comp>()), ...);
}
