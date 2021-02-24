#pragma once

#include <nlohmann/json.hpp>

#include <array>

template<typename Vec>
Vec ReadVec(const nlohmann::json& data) {
	const std::array<std::string, 4> names = { "x", "y", "z", "w" };
	Vec res;
	for (size_t i = 0; i < Vec::length(); i++) {
		if (data.is_array()) {
			res[i] = data[i].get<Vec::value_type>();
		}
		else if (data.is_object()) {
			std::string index = std::to_string(i);
			if (data.find(index) != data.end()) {
				res[i] = data[index].get<Vec::value_type>();
			}
			else {
				res[i] = data[names[i]].get<Vec::value_type>();
			}
		}
	}
	return res;
}

template<typename Vec>
void SaveVec(const Vec& vec, nlohmann::json& data) {
	for (size_t i = 0; i < Vec::length(); i++) {
		data.push_back(vec[i]);
	}
}