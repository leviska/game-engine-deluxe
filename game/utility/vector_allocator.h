#pragma once
#include <vector>
#include <unordered_set>
#include <cassert>

template<typename T>
class VectorAlloc {
public:
	size_t Alloc();
	void Dealloc(size_t id);

	void Clear();

	bool Empty() const { return Size() == 0; }
	size_t Size() const { return data.size() - unused.size(); }
	size_t Capacity() const { return data.size(); }

	bool Check(size_t id) const;

	std::vector<T>& Data() { return data; }
	const std::vector<T>& Data() const { return data; }

	T& operator[](size_t id);
	const T& operator[](size_t id) const;

	T* Raw();
	const T* Raw() const;
private:
	std::vector<T> data;
	std::unordered_set<size_t> unused;
};


template<typename T>
size_t VectorAlloc<T>::Alloc() {
	size_t id = 0;
	if (unused.empty()) {
		id = data.size();
		data.emplace_back();
	}
	else {
		auto it = unused.begin();
		id = *it;
		unused.erase(it);
	}
	return id;
}

template<typename T>
void VectorAlloc<T>::Dealloc(size_t id) {
	unused.insert(id);
}


template<typename T>
void VectorAlloc<T>::Clear() {
	data.clear();
	unused.clear();
}


template<typename T>
T& VectorAlloc<T>::operator[](size_t id) { 
	assert(unused.find(id) == unused.end());
	return data[id]; 
}

template<typename T>
const T& VectorAlloc<T>::operator[](size_t id) const { 
	assert(unused.find(id) == unused.end());
	return data[id];
}

template<typename T>
T* VectorAlloc<T>::Raw() {
	return data.data();
}

template<typename T>
const T* VectorAlloc<T>::Raw() const {
	return data.data();
}

template<typename T>
bool VectorAlloc<T>::Check(size_t id) const {
	return id < data.size() && unused.find(id) == unused.end();
}
