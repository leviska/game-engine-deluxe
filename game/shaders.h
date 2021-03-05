#pragma once

#include "shader.h"
#include "singleton.h"
#include "named_vector.h"

#include <array>

enum class ShadersId {
	Sprite,
	Batch,
	Shapes,
	Particle,
	Light,
	Obstruct,
	Buffer,
	Generate,

	ShadersSize
};

enum class ShaderViewport {
	NoUpdate,
	Normal,
	Reversed,
};

class ShadersImpl : public Singleton {
public:
	NamedVector<Shader> Shaders;
	std::vector<ShaderViewport> Updates;
	
	void Load();
	void Reset();
private:
	~ShadersImpl() { Reset(); }

	friend ShadersImpl& ShadersMut();
};

const ShadersImpl& Shaders();
ShadersImpl& ShadersMut();
