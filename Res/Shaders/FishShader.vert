#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Transform;

uniform float u_Time;
uniform float u_Freq;
uniform float u_Amplitude;
uniform vec3 u_AxisInput;
uniform vec3 u_AxisEffect;

out vec2 vs_textureCoord;

void main()
{
   vec4 pos = position;
   vec4 axisInput = vec4(u_AxisInput, 1.0f);

   pos += vec4(u_AxisEffect * sin(length(axisInput * pos) * u_Freq * u_Time) * u_Amplitude, 0.0f);

   gl_Position = u_Proj * u_View * u_Transform * pos;
   vs_textureCoord = textureCoord;
}