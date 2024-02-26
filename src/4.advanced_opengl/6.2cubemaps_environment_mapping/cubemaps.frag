#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform bool isRefract;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    // 观察方向
    vec3 I = normalize(Position - cameraPos);
    // 反射 or 折射
    vec3 R;
    if (isRefract) {
        R = refract(I, normalize(Normal), 1.00 / 1.52);
    } else {
        R = reflect(I, normalize(Normal));
    }

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
