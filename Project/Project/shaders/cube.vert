#version 400 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 vt;

uniform mat4 MVP;
uniform vec3 eye;
uniform float heightMax;

out vec2 fragTex;
out vec3 normal;
out float z;

void main(){
    normal = vn;
    gl_Position = MVP * vec4(vp, 1.0);
    fragTex = vt;
    z = vp.z;
}