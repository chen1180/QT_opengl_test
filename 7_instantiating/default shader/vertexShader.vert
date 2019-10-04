#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
// Output data ; will be interpolated for each fragment.

// Values that stay constant for the whole mesh.
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 offsets[10000];
out vec2 UV_coord;
void main(){
    vec3 offset=offsets[gl_InstanceID];
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  projection*view*model * vec4(vertexPosition_modelspace+offset,1);
    // UV of the vertex. No special space for this one.
    UV_coord=vertexUV;
//    mat3 normalMat=mat3(transpose(inverse(view*model)));
}
