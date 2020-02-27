#include "Shader.h"

//Code for this class is based off of code from videos 1-26 in this series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2


Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	_shaderID = CreateShader(vertexShaderPath, fragmentShaderPath);
}

Shader::~Shader()
{
	glDeleteProgram(_shaderID);
}

//CompileShader and CreateShader are both adapted from code in these videos https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
GLuint Shader::CompileShader(GLuint type, const std::string& sourcePath)
{
	GLuint shader = glCreateShader(type);

	std::string source;
	std::string line;
	std::ifstream inFile(sourcePath);

	//Check if the file failed to open
	if (inFile.fail())
	{
		std::cerr << "Can't open shader source file " << sourcePath << std::endl;
		return 0;
	}

	//Read in source code
	while (!inFile.eof())
	{
		std::getline(inFile, line);
		source = source + line + '\n';
	}
	inFile.close();

	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	int compileState;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileState);
	if (compileState == GL_FALSE)
	{
		std::cout << "Shader " << sourcePath << " failed to compile" << std::endl;

		//get the length of the error message
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			//create a char array and put the error message into it
			char* errorMessage;
			errorMessage = new char[length];
			glGetShaderInfoLog(shader, length, NULL, errorMessage);

			std::cout << errorMessage << std::endl;

			delete[] errorMessage;
		}
	}

	return shader;
}

//CompileShader and CreateShader are both adapted from code in these videos https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
GLuint Shader::CreateShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderPath);
	fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint result;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE)
	{
		std::cout << "Program failed to validate" << std::endl;

		//get the length of the error message
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		if (length > 0) {
			//create a char array and put the error message into it
			char* errorMessage;
			errorMessage = new char[length];
			glGetProgramInfoLog(program, length, NULL, errorMessage);

			std::cout << errorMessage << std::endl;

			delete[] errorMessage;
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Shader::BindShader()
{
	glUseProgram(_shaderID);
}

int Shader::GetUniformLocation(const std::string& uniformName)
{
	return glGetUniformLocation(_shaderID, uniformName.c_str());
}

void Shader::SetUniformMatrix(glm::mat4& matrix, std::string uniformName)
{
	BindShader();

	int location = GetUniformLocation(uniformName);
	if(location != -1)
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformInt(int value, std::string uniformName)
{
	BindShader();

	int location = GetUniformLocation(uniformName);
	if (location != -1)
		glUniform1i(location, value);
}
