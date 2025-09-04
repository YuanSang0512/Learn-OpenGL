#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 a_Color;

out vec4 m_Color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    m_Color = a_Color;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 o_color;

in vec4 m_Color;

void main()
{
    o_color = m_Color;
};