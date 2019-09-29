#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
out vec4 vertexColor;
uniform vec2 resolution;
uniform vec2 mouse_loc;
uniform float time;
void main(){
    gl_Position = vec4(vertexPosition_modelspace,1.0);
    vertexColor=vec4(abs(cos(time/1000)),abs(sin(time/1000)),0.0,1.0);
}

