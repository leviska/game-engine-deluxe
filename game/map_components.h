#pragma once

#include <string>

#include <nlohmann/json.hpp>

struct FrontWall {};
struct TilingMap {};
struct Rock {};

struct Obstruct {
	inline static const std::string ComponentName{ "Obstruct" };

	void Load(const nlohmann::json& data) {}
	void Save(nlohmann::json& data) const {}
	
	void Edit() {}
};

struct Floor {
	inline static const std::string ComponentName{ "Floor" };

	void Load(const nlohmann::json& data) {}
	void Save(nlohmann::json& data) const {}
	
	void Edit() {}
};

struct Player {
	inline static const std::string ComponentName{ "Player" };

	void Load(const nlohmann::json& data) {}
	void Save(nlohmann::json& data) const {}
	
	void Edit() {}
};

struct Orc {
	inline static const std::string ComponentName{ "Orc" };

	void Load(const nlohmann::json& data) {}
	void Save(nlohmann::json& data) const {}
	
	void Edit() {}
};

struct Ladder {
	inline static const std::string ComponentName{ "Ladder" };

	void Load(const nlohmann::json& data) {}
	void Save(nlohmann::json& data) const {}
	
	void Edit() {}
};

struct Bridge {
	inline static const std::string ComponentName{ "Bridge" };

	void Load(const nlohmann::json& data) {}
	void Save(nlohmann::json& data) const {}
	
	void Edit() {}
};
