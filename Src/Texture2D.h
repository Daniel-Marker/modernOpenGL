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
	bool _isTransparent;

	bool RawLoader(std::string path, int width = 0, int height = 0);
	bool BmpLoader(std::string path);
	bool TgaLoader(std::string path);

public:
	Texture2D();
	~Texture2D();
	void Bind();
	bool Load(std::string path, int width = 0, int height = 0);

	GLuint GetID() const { return _TextureID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	bool GetTransparent() { return _isTransparent; };

};

