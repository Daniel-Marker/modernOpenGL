#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

#include <fstream>
#include <iostream>
#include <string>

class Texture2D
{
protected:
	GLuint _TextureID;
	int _width;
	int _height;
	bool _isTransparent;
	GLenum _textureType;

	void GenerateTexture();
	bool RawLoader(std::string path, int width = 0, int height = 0);
	bool BmpLoader(std::string path);
	bool TgaLoader(std::string path);

public:
	Texture2D(GLenum textureType = GL_TEXTURE_2D);
	~Texture2D();
	void Bind();
	bool Load(std::string path, int width = 0, int height = 0);

	GLuint GetID() const { return _TextureID; }
	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	bool GetTransparent() { return _isTransparent; };

};

