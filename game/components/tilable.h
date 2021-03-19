#pragma once

#include <bitset>

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

enum class TilingId {
	R,
	BRFull,
	BRLeft,
	BRRight,
	BRCorner,
	B,
	BLFull,
	BLLeft,
	BLRight,
	BLCorner,
	L,
	TLFull,
	TLLeft,
	TLRight,
	TLCorner,
	T,
	TRFull,
	TRLeft,
	TRRight,
	TRCorner,
	Size
};

enum class NeighbourId {
	R,
	BR,
	B,
	BL,
	L,
	TL,
	T,
	TR,
	Size
};

using TilingBitset = std::bitset<static_cast<size_t>(TilingId::Size)>;
using NeighbourBitset = std::bitset<static_cast<size_t>(NeighbourId::Size)>;

struct TilableData {
	std::string Name;

	inline static const std::string ComponentName{ "TilableData" };

	void Load(const nlohmann::json& data);
	void Save(nlohmann::json& data) const;

	void Edit();
};

struct TilableInfo {
	uint32_t Id;
};

struct TilingData {
	TilingBitset Data;
};
