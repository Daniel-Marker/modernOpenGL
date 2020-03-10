#include "Cube.h"
Cube::Cube(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera) :
	SceneObject(shader, texture, mesh, material, camera)
{
	_vao = new Vao();
	_vao->BindVao();

	BufferLayout layout;
	
	layoutElement positionsLayout = layoutElement(3, GL_FLOAT, false, mesh->GetVertexCount());
	layout.AddElement(positionsLayout);
	layoutElement textureLayout = layoutElement(2, GL_FLOAT, false, mesh->GetUVCount());
	layout.AddElement(textureLayout);
	layoutElement normalLayout = layoutElement(3, GL_FLOAT, false, mesh->GetVertexNormalCount());
	layout.AddElement(normalLayout);

	_vao->CreateVertexBuffer(_mesh, layout);
	_vao->CreateIndexBuffer(_mesh);

	_shader->SetUniformInt(0, "u_Texture");
}

Cube::~Cube()
{
	delete _vao;
}

void Cube::Render()
{
	//set uniforms
	glm::mat4 transformMatrix =
		glm::translate(glm::mat4(1.0f), _transform.position) *
		glm::eulerAngleXYZ(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z) *
		glm::scale(glm::mat4(1.0f), _transform.scale);
	transformMatrix = _worldTransform * transformMatrix;
	_shader->SetUniformMatrix(transformMatrix, "u_Transform");

	_material->SetMaterialUniforms(_shader);

	Renderer renderer;
	renderer.Render(_vao, _shader, _texture);
}

void Cube::Update(float deltaTime)
{
	if (InputManager::GetKeyDown('z'))
		_transform.rotation.x += 5.0f * deltaTime;
	if (InputManager::GetKeyDown('x'))
		_transform.rotation.y += 5.0f * deltaTime;
	if (InputManager::GetKeyDown('c'))
		_transform.rotation.z += 5.0f * deltaTime;


	SceneObject::Update(deltaTime);
}
