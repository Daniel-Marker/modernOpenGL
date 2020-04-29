#version 450 core

//Code for this shader is based off of code from this tutorial: https://learnopengl.com/Advanced-OpenGL/Cubemaps

in vec3 vs_textureCoord;

out vec4 color;

uniform samplerCube u_Texture;

void main()
{  
    color = texture(u_Texture, vs_textureCoord);
}