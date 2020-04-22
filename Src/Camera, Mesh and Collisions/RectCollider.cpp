#include "RectCollider.h"

RectCollider::RectCollider(glm::vec3 centre, glm::vec3 size, bool isTrigger):
	_centre(centre), _size(size), _isTrigger(isTrigger)
{
}

glm::vec3 RectCollider::GetCurrentAxis(int i, glm::vec3& unitVectorX1, glm::vec3& unitVectorY1, glm::vec3& unitVectorZ1, glm::vec3& unitVectorX2, glm::vec3& unitVectorY2, glm::vec3& unitVectorZ2)
{
	glm::vec3 axis;

	switch (i)
	{
	case 0:
		axis = unitVectorX1;
		break;
	case 1:
		axis = unitVectorY1;
		break;
	case 2:
		axis = unitVectorZ1;
		break;
	case 3:
		axis = unitVectorX2;
		break;
	case 4:
		axis = unitVectorY2;
		break;
	case 5:
		axis = unitVectorZ2;
		break;
	case 6:
		axis = glm::cross(unitVectorX1, unitVectorX2);
		break;
	case 7:
		axis = glm::cross(unitVectorX1, unitVectorY2);
		break;
	case 8:
		axis = glm::cross(unitVectorX1, unitVectorZ2);
		break;
	case 9:
		axis = glm::cross(unitVectorY1, unitVectorX2);
		break;
	case 10:
		axis = glm::cross(unitVectorY1, unitVectorY2);
		break;
	case 11:
		axis = glm::cross(unitVectorY1, unitVectorZ2);
		break;
	case 12:
		axis = glm::cross(unitVectorZ1, unitVectorX2);
		break;
	case 13:
		axis = glm::cross(unitVectorZ1, unitVectorY2);
		break;
	case 14:
		axis = glm::cross(unitVectorZ1, unitVectorZ2);
		break;
	}

	return axis;
}

bool RectCollider::CollisionCheck(const glm::mat4& transformMatrix, RectCollider other, const glm::mat4& otherMatrix)
{
	glm::vec3 otherCentre = other.GetCentre();
	glm::vec3 otherSize = other.GetSize();
	glm::vec3 unitVectorX1, unitVectorY1, unitVectorZ1;
	glm::vec3 unitVectorX2, unitVectorY2, unitVectorZ2;

	unitVectorX1 = glm::normalize(glm::vec3(transformMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	unitVectorY1 = glm::normalize(glm::vec3(transformMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
	unitVectorZ1 = glm::normalize(glm::vec3(transformMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));
	unitVectorX2 = glm::normalize(glm::vec3(otherMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));
	unitVectorY2 = glm::normalize(glm::vec3(otherMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
	unitVectorZ2 = glm::normalize(glm::vec3(otherMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f)));


	glm::vec3 centreToCentreVector = otherMatrix * glm::vec4(otherCentre, 1.0f) - transformMatrix * glm::vec4(_centre, 1.0f);

	for (int i = 0; i < 14; i++)
	{
		glm::vec3 axis = GetCurrentAxis(i, unitVectorX1, unitVectorY1, unitVectorZ1, unitVectorX2, unitVectorY2, unitVectorZ2);

		if (abs(glm::dot(centreToCentreVector, axis)) >
			abs(glm::dot(_size.x * unitVectorX1, axis))
			+ abs(glm::dot(_size.y * unitVectorY1, axis))
			+ abs(glm::dot(_size.z * unitVectorZ1, axis))
			+ abs(glm::dot(otherSize.x * unitVectorX2, axis))
			+ abs(glm::dot(otherSize.y * unitVectorY2, axis))
			+ abs(glm::dot(otherSize.z * unitVectorZ2, axis)))
			return false;
	}

	return true;
}
