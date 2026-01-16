#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 WorldPos;
out vec2 m_TexCoord;
out vec3 Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    WorldPos = vec3(u_Model * vec4(position, 1.0));
    m_TexCoord = texCoord;
    Normal = u_NormalMatrix * normal;
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

in vec3 WorldPos;
in vec2 m_TexCoord;
in vec3 Normal;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_Material.texture_normal1, m_TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(m_TexCoord);
    vec2 st2 = dFdy(m_TexCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    vec3 N = getNormalFromMap();

    vec3 color = texture(u_Material.texture_diffuse1, m_TexCoord).rgb;
    vec3 ambient = 0.1 * color;
    vec3 lightDir = normalize(u_Light.lightPos - WorldPos);
    float diff = max(dot(lightDir, N), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(u_CamPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, N);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(N, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}