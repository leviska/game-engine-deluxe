#pragma once

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include <array>

namespace nlohmann {
	template <size_t N, typename T>
	struct adl_serializer<glm::vec<N, T>> {
		using Vec = glm::vec<N, T>;

		static void from_json(const nlohmann::json& data, Vec& res) {
			const std::array<std::string, 4> names = { "x", "y", "z", "w" };
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
		}

		static void to_json(nlohmann::json& data, const Vec& vec) {
			for (size_t i = 0; i < Vec::length(); i++) {
				data.push_back(vec[i]);
			}
		}
	};
}

void SaveJson(const nlohmann::json& data, const std::string& name);
nlohmann::json LoadJson(const std::string& name);
