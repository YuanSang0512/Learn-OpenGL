#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in mat4 instanceMatrix; // 实例化模型矩阵

out vec2 m_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * instanceMatrix * vec4(position, 1.0);
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

in vec2 m_TexCoord;

void main()
{
    color = texture(u_Material.texture_diffuse1, m_TexCoord);
}