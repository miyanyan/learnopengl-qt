#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform int uPostProcessingId;

uniform sampler2D screenTexture;

// kernel function releated
const float offset = 1.0 / 300.0;

vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // 左上
    vec2( 0.0f,    offset), // 正上
    vec2( offset,  offset), // 右上
    vec2(-offset,  0.0f),   // 左
    vec2( 0.0f,    0.0f),   // 中
    vec2( offset,  0.0f),   // 右
    vec2(-offset, -offset), // 左下
    vec2( 0.0f,   -offset), // 正下
    vec2( offset, -offset)  // 右下
);

vec4 calculateKernel(float kernel[9])
{
    vec3 sampleTex[9];
    for (int i = 0; i < 9; ++i) {
        sampleTex[i] = vec3(texture2D(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; ++i) {
        col += sampleTex[i] * kernel[i];
    }
    return vec4(col, 1.0);
}

vec4 calculateColor(int type)
{
    vec4 color;
    // normal
    if (type == 0) {
        color = texture2D(screenTexture, TexCoords);
    }
    // inversion
    else if (type == 1) {
        color = vec4(vec3(1.0 - texture2D(screenTexture, TexCoords)), 1.0);
    }
    // gray scale
    else if (type == 2) {
        color = texture2D(screenTexture, TexCoords);
        float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
        color = vec4(average, average, average, 1.0);
    }
    // blur
    else if (type == 3) {
        float kernel[9] = float[](
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16
        );
        color = calculateKernel(kernel);
    }
    // edge detection
    else if (type == 4) {
        float kernel[9] = float[](
            1.0, 1.0, 1.0,
            1.0, -8.0, 1.0,
            1.0, 1.0, 1.0
        );
        color = calculateKernel(kernel);
    }
    // sharpen
    else if (type == 5) {
        float kernel[9] = float[](
            -1, -1, -1,
            -1,  9, -1,
            -1, -1, -1
        );
        color = calculateKernel(kernel);
    }
    return color;
}

void main()
{
    FragColor = calculateColor(uPostProcessingId);
}
