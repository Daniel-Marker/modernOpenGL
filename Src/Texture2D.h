#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include <fstream>
#include <iostream>
#include <string>

class Texture2D
{
private:
	GLuint _TextureID;
	int _width;
	int _height;

	bool RawLoader(char* path, int width = 0, int height = 0);
	bool BmpLoader(char* path);
	bool TgaLoader(char* path);

public:
	Texture2D();
	~Texture2D();
	void Bind();
	bool Load(char* path, int width = 0, int height = 0);

	GLuint GetID() const { return _TextureID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};

