#include "SceneObject.h"

float SceneObject::vertexData[] =
{
	1, 1, 1,		1, 0,	0,
	-1, 1, 1,		1, 1,	1,
	-1, -1, 1,		0, 1,	2,
	1, -1, 1,		0, 0,	0,
	1, 1, 1,		1, 0,	1,
	1, -1, 1,		1, 1,	2,
	1, -1, -1,		0, 1,	0,
	1, 1, -1,		0, 0,	1,
	1, 1, 1,		1, 0,	2,
	1, 1, -1,		1, 1,	0,
	-1, 1, -1,		0, 1,	1,
	-1, 1, 1,		0, 0,	2,
	-1, 1, 1,		1, 0,	0,
	-1, 1, -1,		1, 1,	1,
	-1, -1, -1,		0, 1,	2,
	-1, -1, 1,		0, 0,	0,
	-1, -1, -1,		1, 0,	1,
	1, -1, -1,		1, 1,	2,
	1, -1, 1,		0, 1,	0,
	-1, -1, 1,		0, 0,	1,
	1, -1, -1,		1, 0,	2,
	-1, -1, -1,		1, 1,	0,
	-1, 1, -1,		0, 1,	1,
	1, 1, -1,		0, 0,	2
};

unsigned int SceneObject::indices[] =
{	   
	0, 1, 2,
	2, 3, 0,
	4, 5, 6,
	6, 7, 4,
	8, 9, 10,
	10, 11, 8,
	12, 13, 14,
	14, 15, 12,
	16, 17, 18,
	18, 19, 16,
	20, 21, 22,
	22, 23, 20
};

SceneObject::SceneObject(Shader* shader, InputManager* inputManager, Texture2D* texture):
	_shader(shader), _inputManager(inputManager), _texture(texture)
{
	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;
	
	layoutElement positionsLayout = layoutElement(3, GL_FLOAT, false);
	layout.AddElement(positionsLayout);
	layoutElement textureLayout = layoutElement(2, GL_FLOAT, false);
	layout.AddElement(textureLayout);
	layoutElement indexLayout = layoutElement(1, GL_FLOAT, false);
	layout.AddElement(indexLayout);

	_vao->CreateVertexBuffer(vertexData, sizeof(vertexData), layout);

	_vao->CreateIndexBuffer(indices, sizeof(indices));

	_shader->SetUniformInt(0, "u_Texture");
}

SceneObject::~SceneObject()
{
}

void SceneObject::Render()
{
	//set uniforms
	glm::mat4 transformMatrix =
		glm::translate(glm::mat4(1.0f), transform.position) *
		glm::eulerAngleXYZ(transform.rotation.x, transform.rotation.y, transform.rotation.z) *
		glm::scale(glm::mat4(1.0f), transform.scale);
	_shader->SetUniformMatrix(transformMatrix, "u_Transform");

	Renderer renderer;
	renderer.Render(_vao, _shader, _texture);
}

void SceneObject::Update(float deltaTime)
{
	if (_inputManager->GetKeyDown('d'))
		transform.position.x -= 5.0f * deltaTime;

	if (_inputManager->GetKeyDown('a'))
		transform.position.x += 5.0f * deltaTime;

	if (_inputManager->GetKeyDown('w'))
		transform.position.y += 5.0f * deltaTime;

	if (_inputManager->GetKeyDown('s'))
		transform.position.y -= 5.0f * deltaTime;


	if (_inputManager->GetKeyDown('z'))
		transform.rotation.x += 5.0f * deltaTime;
	if (_inputManager->GetKeyDown('x'))
		transform.rotation.y += 5.0f * deltaTime;
	if (_inputManager->GetKeyDown('c'))
		transform.rotation.z += 5.0f * deltaTime;


	if (_inputManager->GetKeyDown('j'))
		transform.scale.x -= 0.5f * deltaTime;
	if (_inputManager->GetKeyDown('k'))
		transform.scale.x += 0.5f * deltaTime;
}
