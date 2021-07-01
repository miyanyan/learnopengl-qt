#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

out vec3 ourColor;

void main(void)
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
