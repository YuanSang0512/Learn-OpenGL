#shader vertex
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D screenTexture;

void main()
{
    vec3 color = texture(screenTexture, v_TexCoord).rgb;
    if(length(color) < 0.001)
        discard;
    color = 1 - color;
    FragColor = vec4(color, 1.0);
};