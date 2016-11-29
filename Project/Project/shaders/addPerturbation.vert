#version 400 core

layout(location = 0) in vec3 vp;

uniform float halfEdgeLength;
uniform vec3 perturbationPoint;
uniform float perturbationValue;
uniform float perturbationMax;

out vec2 texCoords;
out float perturbation;

void main(){
    if(perturbationPoint == vp){
        texCoords = (vec2(vp) + halfEdgeLength * vec2(1.0)) / (2.0*halfEdgeLength);
        perturbation = (perturbationValue / perturbationMax + 1.0) / 2.0;
    } else {
        texCoords = vec2(-1.0);
    }
}

