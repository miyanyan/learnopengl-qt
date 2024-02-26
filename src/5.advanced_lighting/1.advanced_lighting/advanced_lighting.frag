#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform float ambientStrength;
uniform float specularStrength;
uniform float specularShininess;

uniform sampler2D floorTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;

void main()
{
    vec3 color = texture2D(floorTexture, fs_in.TexCoords).rgb;
    //环境光
    vec3 ambient = ambientStrength * color;
    //漫反射光
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color;
    //镜面光
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    float spec = 0.0;
    if (blinn) {
        vec3 halfwayDir  = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), specularShininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess);
    }
    vec3 specular = specularStrength * spec * vec3(0.3); // assuming bright white light color

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
