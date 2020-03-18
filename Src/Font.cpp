#include "Font.h"

static float vertexData[] =
{
	//positions
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,

	//uv coords
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

static int indices[] =
{
	0, 1, 2,
	2, 3, 0
};

Font::Font(std::string fontPath, int characterHeight, int characterWidth, char firstCharacter, Shader* shader):
	_characterHeight(characterHeight), _characterWidth(characterWidth), _firstCharacter(firstCharacter), _shader(shader)
{
	_sourceFont = new Texture2D();
	_sourceFont->Load(fontPath);

	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;

	layoutElement positionsLayout = layoutElement(2, GL_FLOAT, false, 4);
	layoutElement textureLayout = layoutElement(2, GL_FLOAT, false, 4);

	layout.AddElement(positionsLayout);
	layout.AddElement(textureLayout);

	_vao->CreateVertexBuffer(vertexData, sizeof(vertexData), layout);
	_vao->CreateIndexBuffer(indices, sizeof(indices));
}

void Font::OutputString(std::string output, glm::vec2 position, glm::vec2 rotation, glm::vec2 scale)
{
	//set uniforms
	glm::mat4 transformMatrix =
		glm::eulerAngleXYZ(rotation.x, rotation.y, 0.0f) *
		glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

	_shader->SetUniformMatrix(transformMatrix, "u_Transform");

	Renderer renderer;
	renderer.Render(_vao, _shader, _sourceFont);
}
