#include "BufferLayout.h"

//Code for this class is based off of code from videos 1-26 in this series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

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
