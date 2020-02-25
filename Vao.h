#pragma once

#include "GL/glew.h"
#include "GL\freeglut.h"

class Vao
{
private:
	GLuint _vaoID;

public:
	Vao();
	~Vao();

	void BindVao();
};

