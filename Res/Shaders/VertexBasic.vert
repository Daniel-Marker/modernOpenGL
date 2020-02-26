#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in float index;

uniform mat4 u_VP;
uniform mat4 u_Transform;

out vec4 vs_Color;
out vec2 vs_textureCoord;

vec4 colors[] = {vec4(1.0, 0.0, 0.0, 1.0),vec4(0.0, 1.0, 0.0, 1.0),vec4(0.0, 0.0, 1.0, 1.0)};

void main()
{
   //gl_Position = position;
   //vs_Color = colors[gl_VertexID];

   gl_Position = u_VP * u_Transform * position;
   vs_textureCoord = textureCoord;
   vs_Color = colors[int(index)];
}