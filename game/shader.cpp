#include "shader.h"

#include "assertion.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <map>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

uint32_t Shader::currentId = 0;

Shader::Shader(Shader&& other) noexcept {
	id = other.id;
	other.id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
	id = other.id;
	other.id = 0;
	return *this;
}

void Shader::Load(const std::string& vertexFile, const std::string& fragmentFile) {
	id = glCreateProgram();
	uint32_t vertex = LoadShader(vertexFile, Type::Vertex);
	uint32_t fragment = LoadShader(fragmentFile, Type::Fragment);

	Link({ vertex, fragment });
	
	DeleteShader(vertex);
	DeleteShader(fragment);
}

void Shader::Load(const std::string& computeFile) {
	id = glCreateProgram();
	uint32_t compute = LoadShader(computeFile, Type::Compute);

	Link({ compute });

	DeleteShader(compute);
}

void Shader::Reset() {
	glDeleteProgram(id);
	id = 0;
}

void Shader::Select() const {
	currentId = id;
	glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) {
	assert(currentId == id);
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int32_t)value);
}

void Shader::SetInt32(const std::string& name, int32_t value) {
	assert(currentId == id);
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
	assert(currentId == id);
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
	assert(currentId == id);
	glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
	assert(currentId == id);
	glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
	assert(currentId == id);
	glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) {
	assert(currentId == id);
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) {
	assert(currentId == id);
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
	assert(currentId == id);
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetIVec2Vec(const std::string& name, const std::vector<glm::ivec2>& vec) {
	assert(currentId == id);
	glUniform2iv(glGetUniformLocation(id, name.c_str()), vec.size(), glm::value_ptr(vec[0]));
}

void Shader::UpdateProjection(float width, float height) {
	glm::mat4 projection = glm::ortho(0.0f, width, std::abs(std::min(0.0f, height)), std::abs(std::max(0.0f, height)));
	SetMat4("Projection", projection);
}

uint32_t Shader::GetId() const {
	return id;
}

void Shader::Link(const std::vector<uint32_t>& ids) {
	for (auto shaderId : ids) {
		glAttachShader(id, shaderId);
	}

	int success;
	std::string errorLog;
	errorLog.reserve(512);

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, errorLog.capacity(), nullptr, &errorLog[0]);
		THROWERROR("Failed to link shaders\n" + errorLog);
	}

	for (auto shaderId : ids) {
		glDetachShader(id, shaderId);
	}
}

uint32_t Shader::LoadShader(const std::string& fileName, Shader::Type type) {
	std::string code = ReadShader(fileName);
	uint32_t shader = CompileShader(code, type);
	return shader;
}

void Shader::DeleteShader(uint32_t shader) {
	glDeleteShader(shader);
}

std::string Shader::ReadShader(const std::string& fileName) {
	std::stringstream buffer;

	std::ifstream file(fileName);
	if (!file.good()) {
		THROWERROR("Can't open " + fileName + " shader file");
	}

	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

const std::map<Shader::Type, uint32_t> ShaderType = {
	{ Shader::Type::Compute, GL_COMPUTE_SHADER },
	{ Shader::Type::Vertex, GL_VERTEX_SHADER },
	{ Shader::Type::Fragment, GL_FRAGMENT_SHADER },
};

uint32_t Shader::CompileShader(const std::string& code, Shader::Type type) {
	uint32_t shader;
	
	int success;
	std::string errorLog;
	errorLog.reserve(512);

	const char* cstrCode = code.c_str();
	
	shader = glCreateShader(ShaderType.at(type));
	glShaderSource(shader, 1, &cstrCode, nullptr);
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, errorLog.capacity(), nullptr, &errorLog[0]);
		THROWERROR("Failed to compile shader\n" + errorLog);
	};

	return shader;
}
