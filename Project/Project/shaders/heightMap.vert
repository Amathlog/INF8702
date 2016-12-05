#version 400 core

layout(location = 0) in vec3 vp;

uniform sampler2D heightMap;

uniform float halfEdgeLength;
uniform float dx;
uniform float dy;

out vec2 texCoords;
out vec3 pos;

void main(){
    texCoords = vp.xy * 0.5 + 0.5;
    pos = vp;
    gl_Position = vec4(vp, 1.0);
}

