#include <systems/tiling.h>

#include <components/utility.h>
#include <components/movable.h>

#include <resources/graphics.h>

#include <utility/casts.h>

TilingBitset GetTiling(NeighbourBitset neigh) {
	TilingBitset res;

	// middle tiles
	const std::array middleMap = { 
		std::pair{ NeighbourId::R, TilingId::R },
		std::pair{ NeighbourId::B, TilingId::B },
		std::pair{ NeighbourId::L, TilingId::L },
		std::pair{ NeighbourId::T, TilingId::T }
	};

	for (const auto& p : middleMap) {
		res[to_ui32(p.second)] = !neigh[to_ui32(p.first)];
	}

	// corner tiles
	const std::array cornerNeighMap = {
		std::array{ NeighbourId::R, NeighbourId::BR, NeighbourId::B },
		std::array{ NeighbourId::B, NeighbourId::BL, NeighbourId::L },
		std::array{ NeighbourId::L, NeighbourId::TL, NeighbourId::T },
		std::array{ NeighbourId::T, NeighbourId::TR, NeighbourId::R },
	};

	const std::array cornerTileMap = {
		std::array{ TilingId::BRFull, TilingId::BRLeft, TilingId::BRRight, TilingId::BRCorner },
		std::array{ TilingId::BLFull, TilingId::BLLeft, TilingId::BLRight, TilingId::BLCorner },
		std::array{ TilingId::TLFull, TilingId::TLLeft, TilingId::TLRight, TilingId::TLCorner },
		std::array{ TilingId::TRFull, TilingId::TRLeft, TilingId::TRRight, TilingId::TRCorner },
	};

	for (int i = 0; i < 4; i++) {
		int type = neigh[to_ui32(cornerNeighMap[i][0])] * 4 + neigh[to_ui32(cornerNeighMap[i][1])] + neigh[to_ui32(cornerNeighMap[i][2])] * 2;
		TilingId id;
		if (type <= 1) {
			id = cornerTileMap[i][0];
		}
		else if (type >= 2 && type <= 3) {
			id = cornerTileMap[i][1];
		}
		else if (type >= 4 && type <= 5) {
			id = cornerTileMap[i][2];
		}
		else if (type == 6) {
			id = cornerTileMap[i][3];
		}
		else {
			continue;
		}
		res[to_ui32(id)] = true;
	}

	return res;
}

void LoadTiling(entt::registry& reg) {
	reg.clear<TilableInfo>();

	auto view = reg.view<TilableData>();
	const std::vector<std::string>& names = Graphics().TilingNames;
	for (auto id : view) {
		auto [data] = view.get(id);
		
		uint32_t nameId = std::distance(names.begin(), std::find(names.begin(), names.end(), data.Name));
		reg.emplace<TilableInfo>(id, nameId);
	}
}

void UpdateTiling(entt::registry& reg, const MapView& map) {
	reg.clear<TilingData>();

	const std::array shifts = {
		glm::ivec2{ 1, 0 }, glm::ivec2{ 1, 1 }, glm::ivec2{ 0, 1 }, glm::ivec2{ -1, 1 },
		glm::ivec2{ -1, 0 }, glm::ivec2{ -1, -1 }, glm::ivec2{ 0, -1 }, glm::ivec2{ 1, -1 }
	};

	auto view = reg.view<TilableInfo, GridElem>();
	for (auto id : view) {
		auto [info, grid] = view.get(id);
		NeighbourBitset neigh;
		for (int i = 0; i < 8; i++) {
			glm::ivec2 cur = grid.Pos + shifts[i];
			entt::entity other = FirstTypeInMap<TilableInfo>(cur, map, reg);
			neigh[i] = (other != entt::null && reg.get<TilableInfo>(other).Id == info.Id);
		}
		TilingBitset tiling = GetTiling(neigh);
		reg.emplace<TilingData>(id, tiling);
	}
}
