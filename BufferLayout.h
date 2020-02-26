#pragma once

#include <vector>
#include "GL/freeglut.h"

struct layoutElement 
{
	int _size;
	GLenum _type;
	bool _normalized;

	layoutElement(int size, GLenum type, bool normalized):
		_size(size), _type(type), _normalized(normalized)
	{
	}
};

class BufferLayout
{
private:
	std::vector<layoutElement> layout;
	int _stride;
public:
	BufferLayout();
	~BufferLayout();

	const std::vector<layoutElement> GetLayout();
	void AddElement(layoutElement element);
	int GetStride() { return _stride; };
};

