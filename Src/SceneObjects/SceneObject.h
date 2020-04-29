#pragma once

#include <vector>

#include "Structures.h"
#include "InputManager.h"

#include "Shader.h"
#include "Texture2D.h"
#include "Vao.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "RectCollider.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

class Camera;

class SceneObject
{
protected:
	Transform _transform;
	Mesh* _mesh;
	bool _uniqueMesh;

	Vao* _vao;
	Shader* _shader;
	Texture2D* _texture;
	Material* _material;

	Camera* _camera;
	bool _isTransparent;

	std::vector<SceneObject*> _children;
	glm::mat4 _worldTransform;

	RectCollider _collisionRect;

public:
	SceneObject(Shader* shader, Texture2D* texture, Mesh* mesh, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	SceneObject(Shader* shader, Texture2D* texture, std::string meshPath, Material* material, Camera* camera, Transform transform, RectCollider collisionRect);
	virtual ~SceneObject();
	virtual void Render();
	virtual void Update(float deltaTime);
	virtual void OnTrigger();
	const Transform GetTransform();
	void SetTransform(Transform transform);
	bool GetTransparent() { return _isTransparent; };
	void AddChild(SceneObject* child);
	void DeleteChild(int index);
	std::vector<SceneObject*> const GetChildren();
	void GetAllObjects(std::vector<SceneObject*>& transparentObjects, std::vector<SceneObject*>& opaqueObjects);
	void GetAllObjects(std::vector<SceneObject*>& objects);
	void SetChildrenWorldTransform(glm::mat4& worldTransform);

	void UpdateTexture(Texture2D* newTexture) { _texture = newTexture; };

	RectCollider GetRectCollider() { return _collisionRect; };
	glm::mat4 GetTransformMatrix(){
		return _worldTransform *
		glm::translate(glm::mat4(1.0f), _transform.position) *
		glm::eulerAngleXYZ(_transform.rotation.x, _transform.rotation.y, _transform.rotation.z) *
		glm::scale(glm::mat4(1.0f), _transform.scale);
	}

	Camera* GetCamera() { return _camera; };
	Shader* GetShader() { return _shader; };
	Texture2D* GetTexture() { return _texture; };
	Mesh* GetMesh() { return _mesh; };
	Material* GetMaterial() { return _material; };
};

bool distanceComparison(SceneObject* object, SceneObject* other);