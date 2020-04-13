#pragma once

#include "Mesh.h"
#include "Cubemap.h"
#include "Shader.h"
#include "Vao.h"
#include "Renderer.h"

class Skybox
{
private:
	Mesh* _mesh;
	Vao* _vao;
	Shader* _shader;
	Cubemap* _cubemap;

public:
	Skybox(Mesh* mesh, Cubemap* cubemap, Shader* shader);
	void Render();
};

