#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 m_TexCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
    m_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material u_Material;

in vec2 m_TexCoord;

void main()
{
    vec3 diffuse = vec3(texture(u_Material.texture_diffuse1, m_TexCoord));
    vec3 specular = vec3(texture(u_Material.texture_specular1, m_TexCoord));
    color = vec4(diffuse, 1.0);
}