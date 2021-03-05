#pragma once
#include <vector>
#include <unordered_map>
#include <string>

template<typename T>
class NamedVector {
	std::vector<T> values;
	std::unordered_map<std::string, size_t> names;

public:
	size_t Add(const T& value) {
		size_t id = values.size();
		values.push_back(value);
		return id;
	}

	size_t Add(T&& value) {
		size_t id = values.size();
		values.push_back(std::move(value));
		return id;
	}

	size_t Add(const T& value, const std::string& name) {
		size_t id = values.size();
		names[name] = id;
		values.push_back(value);
		return id;
	}

	size_t Add(T&& value, const std::string& name) {
		size_t id = values.size();
		names[name] = id;
		values.push_back(std::move(value));
		return id;
	}

	void AddName(size_t id, const std::string& name) {
		names[name] = id;
	}


	void Clear() {
		values.clear();
		names.clear();
	}


	size_t GetId(const std::string& name) const {
		return names.at(name);
	}

	size_t Size() const {
		return values.size();
	}


	T& operator[](size_t id) {
		return values[id];
	}

	T& operator[](std::string name) {
		return values[names.at(name)];
	}

	const T& operator[](size_t id) const {
		return values[id];
	}

	const T& operator[](std::string name) const {
		return values[names.at(name)];
	}

	typename std::vector<T>::iterator begin() {
		return values.begin();
	}

	typename std::vector<T>::iterator end() {
		return values.end();
	}

	typename std::vector<T>::const_iterator begin() const {
		return values.begin();
	}

	typename std::vector<T>::const_iterator end() const {
		return values.end();
	}
};
