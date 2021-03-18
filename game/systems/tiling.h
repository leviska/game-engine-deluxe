#pragma once

#include <bitset>
#include <array>

#include <components/tiling.h>

TilingBitset GetTiling(NeighbourBitset neigh);

inline const std::array TilingNames = {
	std::pair{ TilingId::R, std::string{"R"} },
	std::pair{ TilingId::BRFull, std::string{"BRFull"} },
	std::pair{ TilingId::BRLeft, std::string{"BRLeft"} },
	std::pair{ TilingId::BRRight, std::string{"BRRight"} },
	std::pair{ TilingId::BRCorner, std::string{"BRCorner"} },
	std::pair{ TilingId::B, std::string{"B"} },
	std::pair{ TilingId::BLFull, std::string{"BLFull"} },
	std::pair{ TilingId::BLLeft, std::string{"BLLeft"} },
	std::pair{ TilingId::BLRight, std::string{"BLRight"} },
	std::pair{ TilingId::BLCorner, std::string{"BLCorner"} },
	std::pair{ TilingId::L, std::string{"L"} },
	std::pair{ TilingId::TLFull, std::string{"TLFull"} },
	std::pair{ TilingId::TLLeft, std::string{"TLLeft"} },
	std::pair{ TilingId::TLRight, std::string{"TLRight"} },
	std::pair{ TilingId::TLCorner, std::string{"TLCorner"} },
	std::pair{ TilingId::T, std::string{"T"} },
	std::pair{ TilingId::TRFull, std::string{"TRFull"} },
	std::pair{ TilingId::TRLeft, std::string{"TRLeft"} },
	std::pair{ TilingId::TRRight, std::string{"TRRight"} },
	std::pair{ TilingId::TRCorner, std::string{"TRCorner"} },
};

