#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Transform;

uniform vec4 u_LightPos;
uniform vec3 u_CameraPos;

uniform vec3 u_DiffuseColor;
uniform float u_DiffuseIntensity;
uniform vec3 u_AmbientColor;
uniform float u_AmbientIntensity;
uniform vec3 u_SpecularColor;
uniform float u_SpecularIntensity;

out vec2 vs_textureCoord;
out vec3 vs_DiffuseLight;
out vec3 vs_AmbientLight;
out vec3 vs_SpecularLight;

void main()
{
    gl_Position = u_Proj * u_View * u_Transform * position;
    vs_textureCoord = textureCoord;

    vec3 LightPos = u_LightPos.xyz;

    vec3 vertexInModelSpace = (u_Transform * position).xyz;
    vec3 norm = normalize((transpose(inverse(u_Transform)) * vec4(normal, 0.0f)).xyz);
    vec3 lightDir = normalize(LightPos - vertexInModelSpace);

    //if w = 0, distanceScale = 1, w = 1, scale = 1/dist
    //done this way to avoid using an if statement
    float distanceFromLight = distance(LightPos, vertexInModelSpace);
    float distanceScale = (1 / distanceFromLight) * mix(distanceFromLight, 1.0f, u_LightPos.w);
    
	vs_DiffuseLight = max(dot(norm, lightDir), 0.0) * u_DiffuseColor * u_DiffuseIntensity * distanceScale;
    vs_AmbientLight = u_AmbientIntensity * u_AmbientColor * distanceScale;
	
	vec3 viewDir = normalize(u_CameraPos - vertexInModelSpace);
	vec3 reflectDir = reflect(-lightDir, norm);
	vs_SpecularLight = u_SpecularIntensity * u_SpecularColor * pow(max(dot(viewDir, reflectDir), 0.0), 2) * distanceScale;
}