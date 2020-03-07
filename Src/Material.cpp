#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess):
	_ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess)
{
}

void Material::SetMaterialUniforms(Shader* shader)
{
    shader->SetUniformVec3(_ambient, "u_Material.ambientColor");
    shader->SetUniformVec3(_diffuse, "u_Material.diffuseColor");
    shader->SetUniformVec3(_specular, "u_Material.specularColor");
    shader->SetUniformFloat(_shininess, "u_Material.shininess");
}
