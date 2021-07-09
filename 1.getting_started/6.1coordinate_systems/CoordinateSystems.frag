#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 ourColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void)
{
    FragColor = mix(texture2D(texture1, TexCoord), texture2D(texture2, TexCoord), 0.4) * vec4(ourColor, 1.0);
}
