#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 u_VP;
uniform mat4 u_Transform;

out vec4 vs_Color;
out vec2 vs_textureCoord;

vec4 colors[] = {vec4(1.0, 0.0, 0.0, 1.0),vec4(1.0, 1.0, 0.0, 1.0),vec4(1.0, 1.0, 1.0, 1.0),vec4(0.0, 1.0, 0.0, 1.0),vec4(0.0, 0.0, 1.0, 1.0)};

void main()
{
   //gl_Position = position;
   gl_Position = u_VP * u_Transform * position;
   //vs_Color = colors[gl_VertexID];
   vs_textureCoord = textureCoord;
}