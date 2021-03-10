#include "gtest/gtest.h"

#include "sorted_alloc.h"

TEST(Unit_Alloc_Sorted, Basic) {
	auto comp = [](int a, int b) {
		return a < b;
	};
	
	SortedAlloc<int, decltype(comp)> arr(comp);

	arr.Push(3);
	arr.Push(1);
	arr.Push(2);
	arr.Push(5);
	size_t id = arr.Push(3);

	arr.Sort();

	ASSERT_EQ(arr.Data(), (std::vector<int>{ 1, 2, 3, 3, 5 }));

	arr.Erase(id);
	arr.Sort();

	ASSERT_EQ(arr.Data(), (std::vector<int>{ 1, 2, 3, 5 }));
}
