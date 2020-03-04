#include "SceneLight.h"
#include "SceneLight.h"

SceneLight::SceneLight(glm::vec3 color, float intensity, glm::vec3 position):
	_color(color), _intensity(intensity), _position(position)
{
}

SceneLight::~SceneLight()
{
}
