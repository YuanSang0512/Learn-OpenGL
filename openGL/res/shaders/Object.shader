#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 m_Color;

out vec3 m_lightColor;
out vec3 m_LightPos;
out float m_AmbientStrength;

out vec3 m_Normal;
out vec3 m_FragPos;

out vec3 m_CamPos;
out float m_specularStrength;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform vec3 u_Color;

uniform vec3 u_lightColor;
uniform vec3 u_LightPos;
uniform float u_ambientStrength;

uniform vec3 u_CamPos;
uniform float u_specularStrength;


void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    m_FragPos = vec3(u_Model * vec4(position, 1.0));
    m_Normal = normalize(mat3(transpose(inverse(u_Model))) * normal);

    m_Color = u_Color;

    m_lightColor = u_lightColor;
    m_LightPos = u_LightPos;
    m_AmbientStrength = u_ambientStrength;

    m_CamPos = u_CamPos;
    m_specularStrength = u_specularStrength;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec3 m_Color;
in vec3 m_Normal;
in vec3 m_FragPos;
in vec3 m_LightPos;
in float m_AmbientStrength;
in vec3 m_lightColor;

in vec3 m_CamPos;
in float m_specularStrength;

void main()
{
    // --- Ambient 环境光 ---
    vec3 ambient = m_AmbientStrength * m_lightColor;

    // --- Diffuse 漫反射 ---
    vec3 lightDir = normalize(m_LightPos - m_FragPos);
    float diff = max(dot(m_Normal, lightDir), 0.0);
    vec3 diffuse = diff * m_lightColor;

    // --- Specular 镜面反射 (Blinn-Phong) ---
    vec3 specular = vec3(0.0);
    if(diff > 0.0) // 只有当面朝向光源时才计算高光
    {
        vec3 viewDir = normalize(m_CamPos - m_FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong 使用半程向量
        float spec = pow(max(dot(m_Normal, halfwayDir), 0.0), 64); // shininess = 32
        specular = m_specularStrength * spec * m_lightColor;
    }

    // --- 最终颜色 ---
    vec3 result = ambient * m_Color + diffuse * m_Color + specular;
    color = vec4(result, 1.0);
}