#pragma once

#include <vector>

#include "GL/glew.h"
#include "GL\freeglut.h"

struct layoutElement 
{
	int _size;
	GLenum _type;
	bool _normalized;
	int _count;

	layoutElement(int size, GLenum type, bool normalized, int count):
		_size(size), _type(type), _normalized(normalized), _count(count)
	{
	}

	unsigned int GetSizeOfType() 
	{
		unsigned int size = 0;

		switch (_type) {
		case GL_BYTE:
			size = sizeof(GLbyte);
			break;

		case GL_UNSIGNED_BYTE:
			size = sizeof(GLubyte);
			break;

		case GL_SHORT:
			size = sizeof(GLshort);
			break;

		case GL_UNSIGNED_SHORT:
			size = sizeof(GLushort);
			break;

		case GL_INT:
			size = sizeof(GLint);
			break;

		case GL_UNSIGNED_INT:
			size = sizeof(GLuint);
			break;

		case GL_FLOAT:
			size = sizeof(GLfloat);
			break;

		case GL_DOUBLE:
			size = sizeof(GLdouble);
			break;

		case GL_BOOL:
			size = sizeof(GLboolean);
			break;

		}

		return size;
	}
};

class BufferLayout
{
private:
	std::vector<layoutElement> layout;
public:
	BufferLayout();
	~BufferLayout();

	const std::vector<layoutElement> GetLayout();
	void AddElement(layoutElement element);
};

