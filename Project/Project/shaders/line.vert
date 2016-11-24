#version 400 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;


uniform mat4 MVP;
out vec3 fragColor;

void main(){

    gl_Position = MVP * vec4(vp, 1.0);
    fragColor = vc;
}