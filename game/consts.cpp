#include "consts.h"

#include "paths.h"
#include "serialization.h"
#include "assertion.h"

#include <nlohmann/json.hpp>

#include <fstream>

void ConstsImpl::Load() {
	DOUT() << "Loading Constants" << std::endl;

	std::ifstream file(Paths::Assets + "consts.json");
	if (!file.good()) {
		THROWERROR("Can't open " + Paths::Assets + "consts.json file");
	}
	
	nlohmann::json j;
	file >> j;

	j.at("CanvasSize").get_to(CanvasSize);
	j.at("TileSize").get_to(TileSize);
	j.at("MaxLayers").get_to(MaxLayers);

	UpdateWindowSize(WindowSize);
	
	DOUT() << "Successfully loaded Constants" << std::endl;
}

void ConstsImpl::Reset() {
	CanvasSize = { 0, 0 };
	Scale = 0;
	TileSize = 0;
}


const ConstsImpl& Consts() {
	return ConstsMut();
}

ConstsImpl& ConstsMut() {
	static ConstsImpl impl;
	return impl;
}

void UpdateWindowSize(glm::uvec2 size) {
	auto& impl = ConstsMut();
	impl.WindowSize = size;
	glm::uvec2 scale = size / impl.CanvasSize;
	impl.Scale = std::min(scale.x, scale.y);
}