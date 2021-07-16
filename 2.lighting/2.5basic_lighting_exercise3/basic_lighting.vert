#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Factor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStrength;
uniform float specularStrength;
uniform float specularShininess;

void main()
{
    //环境光
    vec3 ambient = ambientStrength * lightColor;
    //漫反射光
    vec3 norm = normalize(aNormal);
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
    //为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;

    Factor = ambient + diffuse + specular;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
