#include "Vao.h"

Vao::Vao()
{
	glGenVertexArrays(1, &_vaoID);
}

Vao::~Vao()
{
	glDeleteVertexArrays(1, &_vaoID);
}

void Vao::BindVao()
{
	glBindVertexArray(_vaoID);
}
