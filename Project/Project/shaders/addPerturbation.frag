#version 400 core

uniform sampler2D heightMap;

in vec2 texCoords;
in float perturbation;

out vec4 heightMapColor;

void main(){
    heightMapColor = texture2D(heightMap, texCoords);
    heightMapColor.r += perturbation;
}