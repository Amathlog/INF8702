#version 400 core

layout(location = 0) in vec3 vp;

uniform sampler2D heightMap;

uniform float halfEdgeLength;
uniform float dx;
uniform float dy;

out vec2 texCoords;

void main(){
    texCoords = (vec2(vp) + halfEdgeLength * vec2(1.0)) / (2.0*halfEdgeLength);
}

