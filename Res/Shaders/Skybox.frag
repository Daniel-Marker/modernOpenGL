#version 450 core

in vec3 vs_textureCoord;

out vec4 color;

uniform samplerCube u_Texture;

void main()
{  
    color = texture(u_Texture, vs_textureCoord);
}