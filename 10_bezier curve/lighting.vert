#version 410 core

layout (location = 0) in vec3 Position_VS_in;
layout (location = 1) in vec2 TexCoord_VS_in;

uniform mat4 gWorld;

void main()
{
    gl_Position = vec4(Position_VS_in, 1.0);
}
