#include "BufferLayout.h"

BufferLayout::BufferLayout()
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
}
