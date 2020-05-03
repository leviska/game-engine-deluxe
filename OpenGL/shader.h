#pragma once
#include <string>
#include <cstdint>

class Shader {
	uint32_t id = 0;

public:
	Shader() = default;
	Shader(const std::string& vertexFile, const std::string& fragmentFile);
	~Shader();

	enum class Type {
		None,
		Vertex,
		Fragment
	};

	void Init();
	bool Load(const std::string& vertexFile, const std::string& fragmentFile);

	void Select() const;

	void Set(const std::string& name, bool value) const;
	void Set(const std::string& name, int32_t value) const;
	void Set(const std::string& name, float value) const;

	uint32_t GetId() const;

	bool Link(uint32_t vertex, uint32_t fragment);
	static uint32_t LoadShader(const std::string& fileName, Shader::Type type);
	static void DeleteShader(uint32_t shader);

private:
	static std::string ReadShader(const std::string& fileName);
	static uint32_t CompileShader(const std::string& code, Shader::Type type);
};