#include "SceneObject.h"

float SceneObject::positions[] =
{
	-1, -1, 1,
	1, -1, 1,
	1,-1,-1,
	-1,-1,-1,
	0, 1, 0
};

unsigned int SceneObject::indices[] =
{
	2, 1, 0,
	0, 3, 2,
	1, 4, 0,
	2, 4, 1,
	3, 4, 2,
	0, 4, 3
};

SceneObject::SceneObject(Shader* shaderInput, InputManager* inputManagerInput):
	shader(shaderInput), inputManager(inputManagerInput)
{
	vao = new Vao();
	vao->BindVao();

	//vao needs to have 2 buffer objects for vertex and index buffer and something to say the layout of the vertex buffer

	vao->CreateVertexBuffer(positions, sizeof(positions));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	vao->CreateIndexBuffer(indices, sizeof(indices));
}

SceneObject::~SceneObject()
{
}

void SceneObject::Render(glm::mat4 viewProjMatrix)
{
	shader->BindShader();
	vao->BindVao();

	//set uniforms
	glm::mat4 transformMatrix =
		glm::translate(glm::mat4(1.0f), transform.position) *
		glm::eulerAngleXYZ(transform.rotation.x, transform.rotation.y, transform.rotation.z) *
		glm::scale(glm::mat4(1.0f), transform.scale);
	shader->SetUniformMatrix(transformMatrix, "u_Transform");

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);
}

void SceneObject::Update()
{
	if (inputManager->GetKeyDown('d'))
		transform.position.x -= 0.05f;

	if (inputManager->GetKeyDown('a'))
		transform.position.x += 0.05f;

	if (inputManager->GetKeyDown('w'))
		transform.position.y += 0.05f;

	if (inputManager->GetKeyDown('s'))
		transform.position.y -= 0.05f;


	if (inputManager->GetKeyDown('z'))
		transform.rotation.x += 0.05f;
	if (inputManager->GetKeyDown('x'))
		transform.rotation.y += 0.05f;
	if (inputManager->GetKeyDown('c'))
		transform.rotation.z += 0.05f;


	if (inputManager->GetKeyDown('j'))
		transform.scale.x -= 0.005f;
	if (inputManager->GetKeyDown('k'))
		transform.scale.x += 0.005f;
}
