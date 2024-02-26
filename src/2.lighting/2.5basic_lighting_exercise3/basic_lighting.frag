#version 330 core
in vec3 Factor;
out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(Factor * objectColor, 1.0);
}

