#pragma once

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

struct NeighbourData {
	uint32_t Id;
	NeighbourBitset Data;
};

struct TilingData {
	uint32_t Id;
	TilingBitset Data;
};
