#version 450 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_Proj;
uniform mat4 u_Transform;

out vec2 vs_textureCoord;

void main()
{
   gl_Position = u_Proj * u_Transform * vec4(position, 1.0f, 1.0f);
   vs_textureCoord = textureCoord;
}