#version 450 core

in vec2 vs_textureCoord;
in vec3 vs_DiffuseLight;
in vec3 vs_AmbientLight;

out vec4 color;

uniform sampler2D u_Texture;

void main()
{  
    color = texture(u_Texture, vs_textureCoord) * vec4((vs_DiffuseLight + vs_AmbientLight), 1.0f);
}