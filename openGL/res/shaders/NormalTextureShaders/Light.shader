#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0);
}

#shader fragment
#version 330 core

out vec4 color;

void main()
{
    color = vec4(1.0, 1.0, 1.0, 1.0);
}