#version 400 core

layout(location = 0) out vec4 heightMapColor;

uniform sampler2D heightMap;

in vec2 texCoords;
in float perturbation;

void main(){
    //heightMapColor = texture2D(heightMap, texCoords);
    heightMapColor.r = perturbation;
}