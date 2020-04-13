#pragma once

#include "Buffer.h"
#include "BufferLayout.h"
#include "Vao.h"
#include "Shader.h"
#include "Texture2D.h"

class Renderer
{
private:

public:
	void Render(Vao* vao, Shader* shader, Texture2D* texture);
};

