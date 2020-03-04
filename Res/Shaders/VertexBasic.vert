#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Transform;

out vec2 vs_textureCoord;

void main()
{
   gl_Position = u_Proj * u_View * u_Transform * position;
   vs_textureCoord = textureCoord;
}