#include <resources/shaders.h>

#include <utility/casts.h>
#include <utility/assertion.h>

#include <resources/paths.h>

#include <iostream>

void ShadersImpl::Load() {
	DOUT() << "Loading Shaders" << std::endl;

	LoadShader("sprite_vertex.glsl", "sprite_fragment.glsl", "SpriteShader");
	LoadShader("batch_vertex.glsl", "batch_fragment.glsl", "BatchShader");
	LoadShader("shapes_vertex.glsl", "shapes_fragment.glsl", "ShapesShader");
	LoadShader("particle_vertex.glsl", "particle_fragment.glsl", "ParticleShader");
	LoadShader("light_vertex.glsl", "light_fragment.glsl", "LightShader");
	LoadShader("batch_vertex.glsl", "obstruct_fragment.glsl", "ObstructShader");
	LoadShader("buffer_vertex.glsl", "buffer_fragment.glsl", "BufferShader");

	LoadShader("light_compute.glsl", "GenerateLight");

	DOUT() << "Successfully loaded Shaders" << std::endl;
}

void ShadersImpl::LoadShader(const std::string& vertex, const std::string& fragment, const std::string& name) {
	DOUT() << "Compiling shader \"" << name << "\": \"" << vertex << "\" and \"" << fragment << "\"" << std::endl;

	Shader shader;
	shader.Load(Paths::Shaders + vertex, Paths::Shaders + fragment);
	Shaders.Add(std::move(shader), name);
	
	DOUT() << "Compiled shader \"" << name << "\" successfully" << std::endl;
}

void ShadersImpl::LoadShader(const std::string& compute, const std::string& name) {
	DOUT() << "Compiling shader \"" << name << "\": \"" << compute << "\"" << std::endl;

	Shader shader;
	shader.Load(Paths::Shaders + compute);
	Shaders.Add(std::move(shader), name);

	DOUT() << "Compiled shader \"" << name << "\" successfully" << std::endl;
}


void ShadersImpl::Reset() {
	Shaders.Clear();
}


const ShadersImpl& Shaders() {
	return ShadersMut();
}

ShadersImpl& ShadersMut() {
	static ShadersImpl impl;
	return impl;
}
