#version 330 core
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light{
    //vec3 position;// 使用定向光就不再需要了
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform float matrixLight;
uniform float matrixMove;

uniform vec3 viewPos;

void main()
{
    //环境光
    vec3 ambient = light.ambient * texture2D(material.diffuse, TexCoords).rgb;
    //漫反射光
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * texture2D(material.diffuse, TexCoords).rgb;
    //镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
    //为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture2D(material.specular, TexCoords).rgb;
    // emission
    vec3 emission = vec3(matrixLight) * texture2D(material.emission, vec2(TexCoords.x, TexCoords.y + matrixMove)).rgb;

    FragColor = vec4(ambient + diffuse + specular + emission, 1.0);
}
