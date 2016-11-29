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
uniform vec3 eye;
uniform float halfEdgeLength;

uniform mat4 MVP;

out vec3 fragColor;
//out vec2 texCoord;
out vec3 position;
out vec3 normal;
out float oh;

float computeSinusOffset(){
    float dist = distance(vec2(perturbationPoint), vec2(vp));
    return sin(t + dist / waveLength * M_PI) * maxPerturbation / pow(dist + 1, attenuation);
}



void main(){
    float offset = computeSinusOffset();
    vec3 newPosition = vec3(vp.xy, vp.z + h);
    position = vp;
    normal = vn;
    oh = h;
    gl_Position = MVP * vec4(newPosition, 1.0);
    //fragColor = clamp(vec3(0.0, h/maxPerturbation, 1.0 + h/maxPerturbation), 0.0,1.0);

    /*vec3 incomingRay = vp - eye;
    vec3 refractedRay = refract(incomingRay, vn, 1.0/1.3330);*/
    //refractedRay = incomingRay;
    
    //texCoord = getTexCoords(newPosition, refractedRay, halfEdgeLength);

    //fragColor = vec3((1.0+dot(vec3(0.0, 0.0, 1.0), vec3(vn.x, 0.0, vn.z)))/2.0, (1.0+dot(vec3(0.0, 0.0, 1.0), vec3(0.0, vn.y, vn.z)))/2.0, 1.0);
    //fragColor = vn;
}

