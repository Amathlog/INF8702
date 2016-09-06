#version 430 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec4 fragColor;

void main () {
    gl_Position = P*V*M * vec4 (vp, 1.0);
	fragColor = vec4(vc,1.0);
}