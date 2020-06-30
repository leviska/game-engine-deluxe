#pragma once
#include <cstdint>
#include <string>

class Texture {
	uint32_t id = 0;
	uint32_t width = 0;
	uint32_t height = 0;

public:
	Texture() = default;
	~Texture() { Reset(); }
	Texture(Texture&& other) noexcept;
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Load(const std::string& name);
	void Reset();
	void Select() const;

	uint32_t GetId() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
};