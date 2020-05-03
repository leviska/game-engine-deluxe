#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>

#include <glad/glad.h>

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile) : Shader() {
	Load(vertexFile, fragmentFile);
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::Init() {
	id = glCreateProgram();
}

bool Shader::Load(const std::string& vertexFile, const std::string& fragmentFile) {
	uint32_t vertex = LoadShader(vertexFile, Type::Vertex);
	if (vertex == 0)
		return false;

	uint32_t fragment = LoadShader(fragmentFile, Type::Fragment);
	if (fragment == 0) {
		DeleteShader(vertex);
		return false;
	}

	bool result = Link(vertex, fragment);
	
	DeleteShader(vertex);
	DeleteShader(fragment);

	return result;
}

void Shader::Select() const {
	glUseProgram(id);
}

void Shader::Set(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int32_t)value);
}

void Shader::Set(const std::string& name, int32_t value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::Set(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

uint32_t Shader::GetId() const {
	return id;
}

bool Shader::Link(uint32_t vertex, uint32_t fragment) {
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

	int success;
	std::string errorLog;
	errorLog.reserve(512);

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, errorLog.capacity(), nullptr, &errorLog[0]);
		std::cerr << "Failed to link shaders\n" << errorLog << std::endl;
		return false;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

uint32_t Shader::LoadShader(const std::string& fileName, Shader::Type type) {
	std::string code = ReadShader(fileName);
	if (code.empty())
		return 0;

	uint32_t shader = CompileShader(code, type);
	if (shader == 0)
		return 0;
	return shader;
}

void Shader::DeleteShader(uint32_t shader) {
	glDeleteShader(shader);
}

std::string Shader::ReadShader(const std::string& fileName) {
	std::stringstream buffer;

	std::ifstream file(fileName);
	if (!file.good()) {
		std::cerr << "Can't open " << fileName << " shader file" << std::endl;
		return std::string();
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
		std::cerr << "Failed to compile shader\n" << errorLog << std::endl;
		return 0;
	};

	return shader;
}
