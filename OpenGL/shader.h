#pragma once
#include <string>
#include <cstdint>
#include <glm/glm.hpp>

class Shader {
	// mostly for debug purpose
	static uint32_t currentId;

	uint32_t id = 0;
public:
	Shader() = default;
	~Shader() { Reset(); }
	
	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	enum class Type {
		Vertex,
		Fragment
	};

	void Load(const std::string& vertexFile, const std::string& fragmentFile);
	void Reset();

	void Select() const;

	void SetBool(const std::string& name, bool value);
	void SetInt32(const std::string& name, int32_t value);
	void SetFloat(const std::string& name, float value);
	void SetVec2(const std::string& name, const glm::vec2& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec4(const std::string& name, const glm::vec4& value);
	void SetMat2(const std::string& name, const glm::mat2& value);
	void SetMat3(const std::string& name, const glm::mat3& value);
	void SetMat4(const std::string& name, const glm::mat4& value);

	template <typename T>
	void SetBool(const std::string& name, T value) = delete;
	template <typename T>
	void SetInt32(const std::string& name, T value) = delete;
	template <typename T>
	void SetFloat(const std::string& name, T value) = delete;
	template <typename T>
	void SetVec2(const std::string& name, const T& value) = delete;
	template <typename T>
	void SetVec3(const std::string& name, const T& value) = delete;
	template <typename T>
	void SetVec4(const std::string& name, const T& value) = delete;
	template <typename T>
	void SetMat2(const std::string& name, const T& value) = delete;
	template <typename T>
	void SetMat3(const std::string& name, const T& value) = delete;
	template <typename T>
	void SetMat4(const std::string& name, const T& value) = delete;

	void UpdateProjection(float width, float height);

	uint32_t GetId() const;

	void Link(uint32_t vertex, uint32_t fragment);
	static uint32_t LoadShader(const std::string& fileName, Shader::Type type);
	static void DeleteShader(uint32_t shader);

private:
	static std::string ReadShader(const std::string& fileName);
	static uint32_t CompileShader(const std::string& code, Shader::Type type);
};