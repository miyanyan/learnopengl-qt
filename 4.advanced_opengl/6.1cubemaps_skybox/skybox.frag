#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{

    FragColor = textureCube(skybox, TexCoords);
}
