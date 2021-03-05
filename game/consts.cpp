#include "consts.h"

#include "paths.h"
#include "serialization.h"
#include "assertion.h"

#include <nlohmann/json.hpp>

#include <fstream>

void ConstsImpl::Load() {
	std::ifstream file(Paths::Assets + "consts.json");
	if (!file.good()) {
		THROWERROR("Can't open " + Paths::Assets + "consts.json file");
	}
	
	nlohmann::json j;
	file >> j;

	j.at("CanvasSize").get_to(CanvasSize);
	j.at("Scale").get_to(Scale);
	j.at("TileSize").get_to(TileSize);
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
