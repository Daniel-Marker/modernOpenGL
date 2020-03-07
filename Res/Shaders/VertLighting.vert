#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

struct LightData
{
    vec4 LightPos;

    vec3 DiffuseColor;
    float DiffuseIntensity;

    vec3 AmbientColor;
    float AmbientIntensity;

    vec3 SpecularColor;
    float SpecularIntensity;
};

#define NUM_LIGHTS 2

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Transform;

uniform vec3 u_CameraPos;
uniform LightData[NUM_LIGHTS] u_Lights;

out vec2 vs_textureCoord;
out vec3 vs_DiffuseLight;
out vec3 vs_AmbientLight;
out vec3 vs_SpecularLight;

void main()
{
    gl_Position = u_Proj * u_View * u_Transform * position;
    vs_textureCoord = textureCoord;

    vs_DiffuseLight  = vec3(0.0f, 0.0f, 0.0f);
    vs_AmbientLight  = vec3(0.0f, 0.0f, 0.0f);
    vs_SpecularLight = vec3(0.0f, 0.0f, 0.0f);
    
    for(int i = 0; i < NUM_LIGHTS; i++)
    {
        vec3 LightPos = u_Lights[i].LightPos.xyz;

        vec3 vertexInModelSpace = (u_Transform * position).xyz;
        vec3 norm = normalize((transpose(inverse(u_Transform)) * vec4(normal, 0.0f)).xyz);
        vec3 lightDir = normalize(LightPos - vertexInModelSpace);

        //if w = 0, distanceScale = 1, w = 1, scale = 1/dist
        //done this way to avoid using an if statement
        float distanceFromLight = distance(LightPos, vertexInModelSpace);
        float distanceScale = (1 / distanceFromLight) * mix(distanceFromLight, 1.0f, u_Lights[i].LightPos.w);
        
	    vs_DiffuseLight += max(dot(norm, lightDir), 0.0) * u_Lights[i].DiffuseColor * u_Lights[i].DiffuseIntensity * distanceScale;
        vs_AmbientLight += u_Lights[i].AmbientIntensity * u_Lights[i].AmbientColor * distanceScale;
	    
	    vec3 viewDir = normalize(u_CameraPos - vertexInModelSpace);
	    vec3 reflectDir = reflect(-lightDir, norm);
	    vs_SpecularLight += u_Lights[i].SpecularIntensity * u_Lights[i].SpecularColor * pow(max(dot(viewDir, reflectDir), 0.0), 2) * distanceScale;
    }
}