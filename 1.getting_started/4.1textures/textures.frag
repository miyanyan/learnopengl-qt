#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(void)
{
    FragColor = mix(texture2D(texture1, vec2(1.0 - TexCoord.x, TexCoord.y)), texture2D(texture2, TexCoord), 0.4) * vec4(ourColor, 1.0);
}
