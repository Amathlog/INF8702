#version 400 core

layout(location = 0) in vec3 vp;

uniform float halfEdgeLength;
uniform vec3 perturbationPoint;
uniform float perturbationValue;

out vec2 texCoords;
out float perturbation;

void main(){
    texCoords = (vec2(vp) + halfEdgeLength * vec2(1.0)) / (2.0*halfEdgeLength);
    if(perturbationPoint == vp){
        perturbation = perturbationValue;
    } else {
        perturbation = 0;
    }
    gl_Position = vec4(vp, 1.0);
}

