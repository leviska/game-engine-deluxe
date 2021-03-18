#include <engine/serialization.h>

#include <utility/assertion.h>

#include <fstream>


void SaveJson(const nlohmann::json& data, const std::string& name) {
	std::ofstream file(name);
	if (!file) {
		THROWERROR("Cannot open file " + name);
	}
	file << data;
}

nlohmann::json LoadJson(const std::string& name) {
	nlohmann::json res;
	std::ifstream file(name);
	if (!file.good()) {
		THROWERROR("Cannot open file " + name);
	}
	file >> res;
	return res;
}
