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

class ShadersImpl : public Singleton {
public:
	NamedVector<Shader> Shaders;
	
	void Load();
	void Reset();
private:
	~ShadersImpl() { Reset(); }

	friend ShadersImpl& ShadersMut();
};

const ShadersImpl& Shaders();
ShadersImpl& ShadersMut();
