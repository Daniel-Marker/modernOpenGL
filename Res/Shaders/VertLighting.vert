#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Transform;

uniform vec3 u_LightColor;
uniform float u_LightIntensity;
uniform vec3 u_LightPos;
uniform float u_AmbientLightIntensity;
uniform vec3 u_CameraPos;
uniform float u_SpecularLightIntensity;

out vec2 vs_textureCoord;
out vec3 vs_DiffuseLight;
out vec3 vs_AmbientLight;
out vec3 vs_SpecularLight;

void main()
{
    gl_Position = u_Proj * u_View * u_Transform * position;
    vs_textureCoord = textureCoord;

    vec3 vertexInModelSpace = (u_Transform * position).xyz;
    vec3 norm = normalize((transpose(inverse(u_Transform)) * vec4(normal, 0.0f)).xyz);
    vec3 lightDir = normalize(u_LightPos - vertexInModelSpace);
    
	vs_DiffuseLight = max(dot(norm, lightDir), 0.0) * u_LightColor * u_LightIntensity;
    vs_AmbientLight = u_AmbientLightIntensity * u_LightColor;
	
	vec3 viewDir = normalize(u_CameraPos - vertexInModelSpace);
	vec3 reflectDir = reflect(-lightDir, norm);
	vs_SpecularLight = u_SpecularLightIntensity * u_LightColor * pow(max(dot(viewDir, reflectDir), 0.0), 2);
}