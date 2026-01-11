#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

out vec3 m_FragPos;
out vec2 m_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    m_FragPos = vec3(u_Model * vec4(position, 1.0));
    m_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;

struct Material{
    sampler2D texture_diffuse1;//漫反射贴图(颜色贴图)
    sampler2D texture_normal1;//法线贴图
    float shininess;
};
uniform Material u_Material;

struct Light{
    vec3 lightPos;//光源位置

    vec3 ambient;//环境光
    vec3 diffuse;//漫反射
};
uniform Light u_Light;

uniform vec3 u_CamPos;

in vec3 m_FragPos;
in vec2 m_TexCoord;

void main()
{
    vec3 normalMap = texture(u_Material.texture_normal1, m_TexCoord).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0); // 将法线从 [0,1] 映射到 [-1,1]

    vec3 color = texture(u_Material.texture_diffuse1, m_TexCoord).rgb;
    vec3 ambient = 0.1 * color;
    vec3 lightDir = normalize(u_Light.lightPos - m_FragPos);
    float diff = max(dot(lightDir, normalMap), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(u_CamPos - m_FragPos);
    vec3 reflectDir = reflect(-lightDir, normalMap);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normalMap, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}