#pragma once

#include <graphics/shader.h>

#include <utility/singleton.h>
#include <utility/named_vector.h>

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

	void LoadShader(const std::string& vertex, const std::string& fragment, const std::string& name);
	void LoadShader(const std::string& compute, const std::string& name);

	friend ShadersImpl& ShadersMut();
};

const ShadersImpl& Shaders();
ShadersImpl& ShadersMut();
