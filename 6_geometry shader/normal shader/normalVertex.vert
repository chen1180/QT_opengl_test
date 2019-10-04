#version 330 core
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 vertexNormal;
// Output data ; will be interpolated for each fragment.
out VS_OUT{
    vec3 normal;
}vs_out;
// Values that stay constant for the whole mesh.
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  projection*view*model * vec4(vertexPosition_modelspace,1);
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(projection * vec4(normalMatrix * vertexNormal, 0.0));
}
