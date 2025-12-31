#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 450 core

layout (location = 0) out vec4 color;

in vec3 Normal;
in vec3 Position;

uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;

void main()
{
    vec3 N = normalize(Normal);
    vec3 I = normalize(Position - u_CameraPos);
    vec3 R = reflect(I, normalize(Normal));
    color = vec4(texture(u_Skybox, R).rgb, 1.0);

}