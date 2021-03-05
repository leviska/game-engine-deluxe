#include "Shaders.h"

#include "paths.h"


void ShadersImpl::Load() {
	Shader shader;
	shader.Load(Paths::Shaders + "sprite_vertex.glsl", Paths::Shaders + "sprite_fragment.glsl");
	Shaders.Add(std::move(shader), "SpriteShader");

	shader.Load(Paths::Shaders + "batch_vertex.glsl", Paths::Shaders + "batch_fragment.glsl");
	Shaders.Add(std::move(shader), "BatchShader");

	shader.Load(Paths::Shaders + "shapes_vertex.glsl", Paths::Shaders + "shapes_fragment.glsl");
	Shaders.Add(std::move(shader), "ShapesShader");

	shader.Load(Paths::Shaders + "particle_vertex.glsl", Paths::Shaders + "particle_fragment.glsl");
	Shaders.Add(std::move(shader), "ParticleShader");

	shader.Load(Paths::Shaders + "light_vertex.glsl", Paths::Shaders + "light_fragment.glsl");
	Shaders.Add(std::move(shader), "LightShader");

	shader.Load(Paths::Shaders + "batch_vertex.glsl", Paths::Shaders + "obstruct_fragment.glsl");
	Shaders.Add(std::move(shader), "ObstructShader");

	shader.Load(Paths::Shaders + "buffer_vertex.glsl", Paths::Shaders + "buffer_fragment.glsl");
	Shaders.Add(std::move(shader), "BufferShader");

	shader.Load(Paths::Shaders + "light_compute.glsl");
	Shaders.Add(std::move(shader), "GenerateLight");
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
