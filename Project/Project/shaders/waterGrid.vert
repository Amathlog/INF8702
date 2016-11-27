#version 400 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in float h;

float M_PI = 3.14159265359;

uniform float t;
uniform float maxPerturbation;
uniform float attenuation;
uniform float waveLength;
uniform vec3 perturbationPoint;

uniform mat4 MVP;

out vec3 fragColor;

float computeSinusOffset(){
    float dist = distance(vec2(perturbationPoint), vec2(vp));
    return sin(t + dist / waveLength * M_PI) * maxPerturbation / pow(dist + 1, attenuation);
}

void main(){
    float offset = computeSinusOffset();
    gl_Position = MVP * vec4(vp.xy, vp.z + h, 1.0);
    fragColor = clamp(vec3(0.0, h/maxPerturbation, 1.0 + h/maxPerturbation), 0.0,1.0);
}

