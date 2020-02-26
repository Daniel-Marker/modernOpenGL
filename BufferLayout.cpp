#include "BufferLayout.h"

BufferLayout::BufferLayout():
	_stride(0)
{
}

BufferLayout::~BufferLayout()
{
}

const std::vector<layoutElement> BufferLayout::GetLayout()
{
	return layout;
}

void BufferLayout::AddElement(layoutElement element)
{
	layout.push_back(element);
	_stride += element._size * element.GetSizeOfType();
}
