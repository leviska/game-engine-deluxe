// This is not a test file, but more like console thingy to fast test something

#include "gtest/gtest.h"

#include <entt/entt.hpp>

template<typename Func, typename... Comp, template<typename...> typename List>
void ForEachType(List<Comp...>, Func f) {
	(f(entt::type_identity<Comp>()), ...);
}

struct SimpleComponent {
	int value;

	void Log() {
		std::cout << "SimpleComponent: value = " << value << std::endl;
	}
};

struct EmptyComponent {
	void Log() {
		std::cout << "EmptyComponent" << std::endl;
	}
};

TEST(Sandbox, Usage) {
	entt::registry reg;
	entt::entity id = reg.create();
	
	reg.emplace<SimpleComponent>(id);
	reg.emplace<EmptyComponent>(id);

	auto callback = [&](auto type) {
		using Comp = typename decltype(type)::type; // just copypaste

		if (reg.has<Comp>(id)) {
			if constexpr (std::is_empty_v<Comp>) {
				Comp().Log();
			}
			else {
				reg.get<Comp>(id).Log();
			}
		}
	};

	using ComponentList = entt::type_list<SimpleComponent, EmptyComponent>;
	ForEachType(ComponentList(), callback);
}
