#version 430 core

layout(location = 0) in vec3 vt;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec3 vp;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec4 color;

out vec4 fragColor;

void main () {
    gl_Position = P*V*M * vec4 (vp, 1.0);
	fragColor = color;
}