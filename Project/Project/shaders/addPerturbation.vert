#version 400 core

layout(location = 0) in vec3 vp;

uniform float halfEdgeLength;
uniform vec3 perturbationPoint;
uniform float perturbationValue;

out vec2 texCoords;
out vec3 position;

void main(){
    texCoords = vp.xy * 0.5 + 0.5;
    position = vp;
    gl_Position = vec4(vp, 1.0);
}

