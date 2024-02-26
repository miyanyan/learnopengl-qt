#version 330 core
out vec4 FragColor;

uniform vec4 uColor;//和另一个 vertex shader 的变量不能相同

void main(void)
{
    FragColor = uColor;
}
