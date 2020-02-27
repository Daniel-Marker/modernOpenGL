#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "GL/glew.h"
#include "glm/mat4x4.hpp"

class Shader
{
private:
	GLuint _shaderID;

public:
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	~Shader();

	GLuint CompileShader(GLuint type, const std::string& sourcePath);
	GLuint CreateShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	void BindShader();
	int GetUniformLocation(const std::string& uniformName);
	void SetUniformMatrix(glm::mat4& matrix, std::string uniformName);
	void SetUniformInt(int value, std::string uniformName);
};

