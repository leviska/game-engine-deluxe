#include "shader.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <cassert>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void Shader::Load(const std::string& vertexFile, const std::string& fragmentFile) {
	id = glCreateProgram();
	uint32_t vertex = LoadShader(vertexFile, Type::Vertex);
	uint32_t fragment = LoadShader(fragmentFile, Type::Fragment);

	Link(vertex, fragment);
	
	DeleteShader(vertex);
	DeleteShader(fragment);
}

void Shader::Reset() {
	glDeleteProgram(id);
}

void Shader::Select() const {
	glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int32_t)value);
}

void Shader::SetInt32(const std::string& name, int32_t value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
	glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
	glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& value) {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat3(const std::string& name, const glm::mat3& value) {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::UpdateProjection(float width, float height) {
	glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height, -100.0f, 100.0f);
	SetMat4("projection", projection);
}

uint32_t Shader::GetId() const {
	return id;
}

void Shader::Link(uint32_t vertex, uint32_t fragment) {
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

	int success;
	std::string errorLog;
	errorLog.reserve(512);

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, errorLog.capacity(), nullptr, &errorLog[0]);
		throw std::runtime_error("Failed to link shaders\n" + errorLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
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
		throw std::runtime_error("Can't open " + fileName + " shader file");
	}

	buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

uint32_t Shader::CompileShader(const std::string& code, Shader::Type type) {
	assert(type != Type::None);
	uint32_t shader;
	
	int success;
	std::string errorLog;
	errorLog.reserve(512);

	const char* cstrCode = code.c_str();
	
	shader = glCreateShader((type == Type::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
	glShaderSource(shader, 1, &cstrCode, nullptr);
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, errorLog.capacity(), nullptr, &errorLog[0]);
		throw std::runtime_error("Failed to compile shader\n" + errorLog);
	};

	return shader;
}
