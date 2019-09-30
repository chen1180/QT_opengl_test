#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
out vec3 fragColor;
uniform vec2 resolution;
uniform vec2 mouse_loc;
uniform float time;
uniform mat4 MVP;
void main(){
    gl_Position =MVP*vec4(vertexPosition_modelspace,1.0);
    fragColor=vertexColor;
}

