#pragma once

#include <vector>
#include <numeric>
#include <algorithm>
#include <unordered_map>

template<typename T, typename Comp>
class SortedAlloc {
public:
	SortedAlloc(const Comp& comp) : compare(comp) {}

	size_t Push(const T& value);
	size_t Push(T&& value);
	void Erase(size_t id);

	void Sort();

	std::vector<T>& Data() { return data; }
	std::vector<T>& Data() const { return data; }

	T& operator[](size_t id) { return data[indexes.at(id)]; }
	const T& operator[](size_t id) const { return data[indexes.at(id)]; }
private:
	void Swap(size_t a, size_t b);

	Comp compare;
	std::vector<T> data;
	std::vector<size_t> ids;
	std::unordered_map<size_t, size_t> indexes;
	size_t lastId = 0;
};

template<typename T, typename Comp>
size_t SortedAlloc<T, Comp>::Push(const T& value) {
	ids.push_back(lastId);
	indexes[lastId] = data.size();
	data.push_back(value);
	return lastId++;
}

template<typename T, typename Comp>
size_t SortedAlloc<T, Comp>::Push(T&& value) {
	ids.push_back(lastId);
	indexes[lastId] = data.size();
	data.push_back(std::move(value));
	return lastId++;
}

template<typename T, typename Comp>
void SortedAlloc<T, Comp>::Erase(size_t id) {
	size_t oldIndex = indexes.at(id);
	if (oldIndex != data.size()) {
		size_t lastId = ids.back();
		indexes[lastId] = oldIndex;
		ids[oldIndex] = lastId;
		std::swap(data[oldIndex], data.back());
	}
	data.pop_back();
	ids.pop_back();
	indexes.erase(id);
}

template<typename T, typename Comp>
void SortedAlloc<T, Comp>::Sort() {
	std::vector<size_t> perm(data.size());
	std::iota(perm.begin(), perm.end(), 0);
	std::sort(perm.begin(), perm.end(), [&](size_t a, size_t b) {
		return compare(data[a], data[b]);
	});
	for (size_t i = 0; i < perm.size(); i++) {
		if (perm[i] >= perm.size()) {
			continue;
		}
		size_t j = i;
		do {
			Swap(j, perm[j]);
			size_t t = j;
			j = perm[j];
			perm[t] = perm.size();
		} while (j != i);
	}
}

template<typename T, typename Comp>
void SortedAlloc<T, Comp>::Swap(size_t a, size_t b) {
	std::swap(data[a], data[b]);
	std::swap(ids[a], ids[b]);
	std::swap(indexes.at(a), indexes.at(b));
}