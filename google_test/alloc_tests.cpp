#include <gtest/gtest.h>

#include <sorted_alloc.h>

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

TEST(Unit_Alloc_Sorted, IndexesCorrect) {
	auto comp = [](int a, int b) {
		return a < b;
	};

	int n = 10;
	SortedAlloc<int, decltype(comp)> arr(comp);
	std::vector<int> indexes(n);
	std::vector<int> perm{ 0, 3, 6, 2, 4, 1, 9, 5, 7, 8 };

	ASSERT_EQ(perm.size(), n);

	for (int i = 0; i < n; i++) {
		indexes[perm[i]] = arr.Push(perm[i]);
	}

	for (int i = 0; i < n; i++) {
		ASSERT_EQ(arr[indexes[i]], i);
	}

	arr.Sort();
	ASSERT_TRUE(std::is_sorted(arr.Data().begin(), arr.Data().end()));

	for (int i = 0; i < n; i++) {
		ASSERT_EQ(arr[indexes[i]], i);
	}

	for (int i = 0; i < n; i++) {
		arr.Erase(indexes[i]);
		arr.Sort();
		ASSERT_TRUE(std::is_sorted(arr.Data().begin(), arr.Data().end()));
	}

}

TEST(Unit_Alloc_Sorted, Shifted) {
	auto comp = [](int a, int b) {
		return a < b;
	};

	int n = 5;
	SortedAlloc<int, decltype(comp)> arr(comp);
	std::vector<int> indexes(n);
	std::vector<int> perm{ 1, 2, 3, 4, 0 };

	ASSERT_EQ(perm.size(), n);

	for (int i = 0; i < n; i++) {
		indexes[perm[i]] = arr.Push(perm[i]);
	}

	for (int i = 0; i < n; i++) {
		ASSERT_EQ(arr[indexes[i]], i);
	}

	arr.Sort();
	ASSERT_TRUE(std::is_sorted(arr.Data().begin(), arr.Data().end()));

	for (int i = 0; i < n; i++) {
		ASSERT_EQ(arr[indexes[i]], i);
	}
}
