#pragma once

#include <graphics/camera.h>
#include <graphics/sprite_ptr.h>

#include <utility/sorted_alloc.h>

#include <resources/graphics.h>

#include <vector>
#include <unordered_map>

class Renderer {
	struct CompareSprites {
		bool operator()(const Sprite& a, const Sprite& b) { return a.Layer < b.Layer; }
	};

	static constexpr uint32_t MaxTextures = 4;
public:
	Renderer();

	void Load();
	void Reset();

	SpritePtr Stage(const Sprite& sprite, size_t textureId);
	SpritePtr Stage(size_t textureId);
	SpritePtr Stage(const SpriteInfo& info);
	SpritePtr Stage(const std::string& name);

	size_t StageRaw(Sprite sprite, size_t textureId);

	Sprite& operator[](size_t id);
	const Sprite& operator[](size_t id) const;

	void Unstage(size_t id);

	void Draw(const Camera& camera);
private:
	SortedAlloc<Sprite, CompareSprites> data;
	std::vector<size_t> textureIds;
	std::unordered_map<size_t, size_t> textSearch;
	uint32_t dataBuffer = 0;

	void AddTextureIfNeeded(size_t textId);
};
