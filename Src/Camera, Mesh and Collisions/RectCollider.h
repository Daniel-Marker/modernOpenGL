#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include <vector>
#include <limits>

#include <iostream>

class RectCollider
{
private:
	glm::vec3 _centre;
	glm::vec3 _size;
	bool _isTrigger;

	glm::vec3 GetCurrentAxis(int i, glm::vec3& unitVectorX1, glm::vec3& unitVectorY1, glm::vec3& unitVectorZ1, glm::vec3& unitVectorX2, glm::vec3& unitVectorY2, glm::vec3& unitVectorZ2);

public:
	RectCollider(glm::vec3 centre, glm::vec3 size, bool isTrigger = false);
	bool CollisionCheck(const glm::mat4& transformMatrix,RectCollider other, const glm::mat4& otherMatrix);

	glm::vec3 GetCentre() { return _centre; }
	glm::vec3 GetSize() { return _size; }
	bool IsTrigger() { return _isTrigger; }
};

