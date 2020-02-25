#pragma once

#include <vector>
#include "GL/freeglut.h"

struct layoutElement 
{
	int _size;
	GLenum _type;
	bool _normalized;
	int _stride;

	layoutElement(int size, GLenum type, bool normalized, int stride):
		_size(size), _type(type), _normalized(normalized), _stride(stride)
	{
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

