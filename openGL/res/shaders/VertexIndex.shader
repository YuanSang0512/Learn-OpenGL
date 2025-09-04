#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in float textureIndex;

out vec2 v_TexCoord;
out float v_TextureIndex;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_TextureIndex = textureIndex;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TextureIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TextureIndex);
    color = texture(u_Textures[index], v_TexCoord);
}