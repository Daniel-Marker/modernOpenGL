#version 450 core

//Code for this shader is based off of code from this tutorial: https://learnopengl.com/Advanced-OpenGL/Cubemaps

layout (location = 0) in vec3 position;

uniform mat4 u_Proj;
uniform mat4 u_View;

out vec3 vs_textureCoord;

void main()
{
   vs_textureCoord = position.xyz;
   vec4 pos = u_Proj * u_View * vec4(position.x, -position.y, position.z, 1.0);
   gl_Position = pos.xyww;
}