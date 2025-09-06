#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 m_FragPos;
out vec3 m_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    m_FragPos = vec3(u_Model * vec4(position, 1.0));
    m_Normal = mat3(transpose(inverse(u_Model))) * normal;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

struct Material{
    vec3 ambient;//��������
    vec3 diffuse;//������
    vec3 specular;//���淴��
    float shininess;//�߹�ϵ��
 };
uniform Material u_material;

struct Light{
    vec3 lightPos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light u_Light;

uniform vec3 u_Color;

uniform vec3 u_lightColor;

uniform vec3 u_CamPos;

in vec3 m_FragPos;
in vec3 m_Normal;

void main()
{
    vec3 norm = normalize(m_Normal);
    // --- ambient ������ ---
    vec3 f_ambient = u_Light.ambient * u_material.ambient * u_lightColor;

    // --- diffuse ������ ---
    vec3 lightDir = normalize(u_Light.lightPos - m_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 f_diffuse = u_Light.diffuse * u_lightColor * (diff * u_material.diffuse);

    // --- specular ���淴�� (Blinn-Phong) ---
    vec3 f_specular = vec3(0.0);
    if(diff > 0.0) // ֻ�е��泯���Դʱ�ż���߹�
    {
        vec3 viewDir = normalize(u_CamPos - m_FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir); // Blinn-Phong ʹ�ð������
        float spec = pow(max(dot(norm, halfwayDir), 0.0), u_material.shininess);
        f_specular = u_Light.specular * u_material.specular * spec * u_lightColor;
    }
    //f_specular = vec3(0.0);


    // --- ������ɫ ---
    vec3 result = f_ambient * u_Color + f_diffuse * u_Color + f_specular;
    //vec3 result = f_ambient + f_diffuse + f_specular;
    color = vec4(result, 1.0);
}