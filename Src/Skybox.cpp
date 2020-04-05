#include "Skybox.h"

Skybox::Skybox(Mesh* mesh, Cubemap* cubemap, Shader* shader):
	_mesh(mesh), _cubemap(cubemap), _shader(shader)
{
	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;

	layoutElement positionsLayout = layoutElement(3, GL_FLOAT, false, mesh->GetVertexCount());
	layout.AddElement(positionsLayout);

	_vao->CreateVertexBuffer(_mesh, layout);
	_vao->CreateIndexBuffer(_mesh);
}

void Skybox::Render()
{
	Renderer renderer;
	renderer.Render(_vao, _shader, _cubemap);
}
