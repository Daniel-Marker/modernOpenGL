#include "Renderer.h"

void Renderer::Render(Vao* vao, Shader* shader)
{
	shader->BindShader();
	vao->BindVao();

	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetSize(), GL_UNSIGNED_INT, nullptr);
}
