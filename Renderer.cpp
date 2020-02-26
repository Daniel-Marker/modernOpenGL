#include "Renderer.h"

void Renderer::Render(Vao* vao, Shader* shader, Texture2D* texture)
{
	shader->BindShader();
	vao->BindVao();
	texture->Bind();

	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetSize(), GL_UNSIGNED_INT, nullptr);
}
