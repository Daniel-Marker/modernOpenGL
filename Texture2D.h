#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

class Texture2D
{
private:
	GLuint _TextureID;
	int _width;
	int _height;

public:
	Texture2D();
	~Texture2D();
	void Bind();
	bool Load(char* path, int width, int height);

	GLuint GetID() const { return _TextureID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};

