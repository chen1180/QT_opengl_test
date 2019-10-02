#version 330 core
out vec3 outputColor;
in vec2 UV;
uniform sampler2D myTextureSampler;

void main(){
    outputColor=texture2D(myTextureSampler, UV ).rgb;
}
