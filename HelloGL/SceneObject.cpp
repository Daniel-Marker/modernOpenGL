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

SceneObject::SceneObject(GLuint shaderInput, InputManager* inputManagerInput):
	shader(shaderInput), inputManager(inputManagerInput)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	unsigned int indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

}

SceneObject::~SceneObject()
{
}

void SceneObject::Render(glm::mat4 viewProjMatrix)
{
	glUseProgram(shader);
	glBindVertexArray(vao);

	//set uniforms
	int location = glGetUniformLocation(shader, "u_Transform");
	if (location != -1)
	{
		glm::mat4 transformMatrix =
			glm::translate(glm::mat4(1.0f), transform.position) *
			glm::eulerAngleXYZ(transform.rotation.x, transform.rotation.y, transform.rotation.z) *
			glm::scale(glm::mat4(1.0f), transform.scale);
		glUniformMatrix4fv(location, 1, GL_FALSE, &transformMatrix[0][0]);
	}
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
