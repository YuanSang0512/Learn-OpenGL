#shader vertex
#version 330 core

layout (location = 0) in vec4 position;

out vec3 m_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec3 u_Color;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * position;
    m_Color = u_Color;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec3 m_Color;

void main()
{
    color = vec4(m_Color, 1.0);
}