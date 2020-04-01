#pragma once

#include <vector>

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Texture2D.h"
#include "Vao.h"
#include "Mesh.h"
#include "Material.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class SceneObject
{
protected:
	Transform _transform;
	Mesh* _mesh;
	
	Vao* _vao;
	Shader* _shader;
	Texture2D* _texture;
	Material* _material;

	Camera* _camera;
	bool _isTransparent;

	std::vector<SceneObject*> _children;
	glm::mat4 _worldTransform;

public:
	SceneObject(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform);
	virtual ~SceneObject();
	virtual void Render();
	virtual void Update(float deltaTime);
	const Transform GetTransform();
	void SetTransform(Transform transform);
	bool GetTransparent() { return _isTransparent; };
	void AddChild(SceneObject* child);
	void DeleteChild(int index);
	std::vector<SceneObject*> const GetChildren();
	void GetAllObjects(std::vector<SceneObject*>& transparentObjects, std::vector<SceneObject*>& opaqueObjects);
	void SetChildrenWorldTransform(glm::mat4& worldTransform);

	bool operator< (const SceneObject& other) const;
};

