#include "Renderer.h"

//Code for this class is based off of code from videos 1-26 in this series: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2

void Renderer::Render(Vao* vao, Shader* shader, Texture2D* texture)
{
	shader->BindShader();
	vao->BindVao();
	texture->Bind();

	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetSize(), GL_UNSIGNED_INT, nullptr);
}
