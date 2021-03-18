#pragma once
#include <string>
#include <cstdint>
#include <vector>
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
		Fragment,
		Compute
	};

	void Load(const std::string& vertexFile, const std::string& fragmentFile);
	void Load(const std::string& computeFile);
	void Reset();

	void Select() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt32(const std::string& name, int32_t value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetMat2(const std::string& name, const glm::mat2& value) const;
	void SetMat3(const std::string& name, const glm::mat3& value) const;
	void SetMat4(const std::string& name, const glm::mat4& value) const;
	void SetIVec2Vec(const std::string& name, const std::vector<glm::ivec2>& vec) const;

	template <typename T>
	void SetBool(const std::string& name, T value) const = delete;
	template <typename T>
	void SetInt32(const std::string& name, T value) const = delete;
	template <typename T>
	void SetFloat(const std::string& name, T value) const = delete;
	template <typename T>
	void SetVec2(const std::string& name, const T& value) const = delete;
	template <typename T>
	void SetVec3(const std::string& name, const T& value) const = delete;
	template <typename T>
	void SetVec4(const std::string& name, const T& value) const = delete;
	template <typename T>
	void SetMat2(const std::string& name, const T& value) const = delete;
	template <typename T>
	void SetMat3(const std::string& name, const T& value) const = delete;
	template <typename T>
	void SetMat4(const std::string& name, const T& value) const = delete;
	template <typename T>
	void SetIVec2Vec(const std::string& name, const T& vec) const = delete;


	void UpdateProjection(glm::vec2 size, bool flip) const;

	uint32_t GetId() const;

	void Link(const std::vector<uint32_t>& ids);
	static uint32_t LoadShader(const std::string& fileName, Shader::Type type);
	static void DeleteShader(uint32_t shader);

private:
	static std::string ReadShader(const std::string& fileName);
	static uint32_t CompileShader(const std::string& code, Shader::Type type);
};