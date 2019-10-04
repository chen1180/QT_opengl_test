#version 410 core

in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;
in vec3 WorldPos_FS_in;

out vec3 FragColor;

uniform sampler2D gColorMap;

void main()
{
    FragColor = texture2D(gColorMap, TexCoord_FS_in.xy).rgb;
}
