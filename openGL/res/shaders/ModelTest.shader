#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 m_FragPos;
out vec3 m_Normal;
out vec2 m_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    m_FragPos = vec3(u_Model * vec4(position, 1.0));
    m_Normal = mat3(transpose(inverse(u_Model))) * normal;
    m_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

struct Material{
    sampler2D texture_diffuse1;//漫反射贴图(颜色贴图)
    sampler2D texture_specular1;//镜面反射贴图
    float shininess;
};
uniform Material u_Material;

struct Light{
    vec3 lightPos;//光源位置
    //三种光的影响强度控制
    vec3 ambient;//环境光
    vec3 diffuse;//漫反射
    vec3 specular;//镜面反射
};
uniform Light u_Light;

uniform vec3 u_CamPos;

in vec3 m_FragPos;
in vec3 m_Normal;

in vec2 m_TexCoord;

void main()
{
    vec3 f_ambient = u_Light.ambient * vec3(texture(u_Material.texture_diffuse1, m_TexCoord));

    vec3 norm = normalize(m_Normal);
    vec3 lightDir = normalize(u_Light.lightPos - m_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 f_diffuse = u_Light.diffuse * diff * vec3(texture(u_Material.texture_diffuse1, m_TexCoord));

    vec3 f_specular = vec3(0.0);
    if(diff > 0.0) // 只有当面朝向光源时才计算高光
    {
        vec3 viewDir = normalize(u_CamPos - m_FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong 使用半程向量
        float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Material.shininess);
        f_specular = u_Light.specular * spec * vec3(texture(u_Material.texture_specular1, m_TexCoord));
    }

    vec3 result = f_ambient + f_diffuse + f_specular;
    color = vec4(result, 1.0);
}